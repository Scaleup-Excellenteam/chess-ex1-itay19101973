#include "MoveRecommender/MoveRecommender.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <climits>

MoveRecommender::MoveRecommender(Board& board, int maxDepth)
    : m_board(board), m_maxDepth(maxDepth), m_isWhiteTurn(true),
    m_whiteMoveQueue(0), m_blackMoveQueue(0) {
    // Initialize with empty queues
}

std::string MoveRecommender::coordinatesToNotation(int row, int col) const {
    char file = 'A' + row;  // Convert row to file (A-H)
    char rank = '1' + col;  // Convert col to rank (1-8)
    return std::string(1, file) + std::string(1, rank);
}

std::vector<ChessMove> MoveRecommender::generateValidMoves(bool forWhite) const {
    std::vector<ChessMove> validMoves;

    // Iterate through the entire board
    for (int srcRow = 0; srcRow < 8; srcRow++) {
        for (int srcCol = 0; srcCol < 8; srcCol++) {
            // Check if there's a piece at this position
            std::shared_ptr<Piece> piece = m_board.getPieceAt(srcRow, srcCol);

            // Skip if no piece or if piece color doesn't match the requested side
            if (!piece || piece->getIsWhite() != forWhite) {
                continue;
            }

            std::string source = coordinatesToNotation(srcRow, srcCol);

            // Try all possible destinations
            for (int destRow = 0; destRow < 8; destRow++) {
                for (int destCol = 0; destCol < 8; destCol++) {
                    std::string dest = coordinatesToNotation(destRow, destCol);

                    // Check if the move is valid
                    int moveCode = m_board.validateMove(source, dest);
                    if (moveCode == 41 || moveCode == 42) {
                        // Valid move - add to list
                        validMoves.emplace_back(source, dest, forWhite);
                    }
                }
            }
        }
    }

    return validMoves;
}

bool MoveRecommender::isMoveStillValid(const ChessMove& move) const {
    int moveCode = m_board.validateMove(move.sourcePos, move.destPos);
    return (moveCode == 41 || moveCode == 42);
}

void MoveRecommender::refreshMoveQueues(int topN) {
    // Clear and rebuild the queue for the current player
    if (m_isWhiteTurn) {
        m_whiteMoveQueue = PriorityQueue<ChessMove, ChessMoveComparator>(topN);

        // Generate all valid white moves
        std::vector<ChessMove> whiteMoves = generateValidMoves(true);

        for (auto& move : whiteMoves) {
            try {
                move.score = evaluateMove(move, m_maxDepth, true);
                m_whiteMoveQueue.push(move);
            }
            catch (const std::exception& e) {
                std::cerr << "Error evaluating white move " << move.toString()
                    << ": " << e.what() << std::endl;
            }
        }
    }
    else {
        m_blackMoveQueue = PriorityQueue<ChessMove, ChessMoveComparator>(topN);

        // Generate all valid black moves
        std::vector<ChessMove> blackMoves = generateValidMoves(false);

        for (auto& move : blackMoves) {
            try {
                move.score = evaluateMove(move, m_maxDepth, true);
                m_blackMoveQueue.push(move);
            }
            catch (const std::exception& e) {
                std::cerr << "Error evaluating black move " << move.toString()
                    << ": " << e.what() << std::endl;
            }
        }
    }
}

void MoveRecommender::updateCachedMoves(const std::string& source, const std::string& dest) {
    // Find if the played move was in our recommendations
    ChessMove playedMove(source, dest, m_isWhiteTurn);

    // Toggle player turn after a move
    m_isWhiteTurn = !m_isWhiteTurn;

    // Both queues need validation since the board state changed
    PriorityQueue<ChessMove, ChessMoveComparator> newWhiteQueue(m_whiteMoveQueue.size());
    PriorityQueue<ChessMove, ChessMoveComparator> newBlackQueue(m_blackMoveQueue.size());

    // Revalidate white moves
    for (const ChessMove& move : m_whiteMoveQueue.getList()) {
        // Skip the move that was just played
        if (move == playedMove) continue;

        // Check if the move is still valid
        if (isMoveStillValid(move)) {
            newWhiteQueue.push(move);
        }
    }

    // Revalidate black moves
    for (const ChessMove& move : m_blackMoveQueue.getList()) {
        // Skip the move that was just played
        if (move == playedMove) continue;

        // Check if the move is still valid
        if (isMoveStillValid(move)) {
            newBlackQueue.push(move);
        }
    }

    // Update the queues with validated moves
    m_whiteMoveQueue = std::move(newWhiteQueue);
    m_blackMoveQueue = std::move(newBlackQueue);
}

int MoveRecommender::getPieceValue(char pieceSymbol) const {
    switch (tolower(pieceSymbol)) {
    case 'p': return 1;  // Pawn
    case 'n': return 3;  // Knight
    case 'b': return 3;  // Bishop
    case 'r': return 5;  // Rook
    case 'q': return 9;  // Queen
    case 'k': return 100; // King
    default: return 0;   // Empty or invalid
    }
}

int MoveRecommender::evaluateCapture(const ChessMove& move) const {
    auto [destRow, destCol] = m_board.notationToCoordinates(move.destPos);
    std::shared_ptr<Piece> destPiece = m_board.getPieceAt(destRow, destCol);

    if (destPiece) {
        // Piece value approximation
        char capturedSymbol = destPiece->getSymbol();
        return getPieceValue(capturedSymbol) * 10;
    }

    return 0; // No capture
}

int MoveRecommender::evaluateCheck(int moveCode) const {
    // If this move puts opponent in check, add points
    return (moveCode == 41) ? 5 : 0;
}

int MoveRecommender::evaluateThreat(int row, int col, bool isWhite, int pieceValue) {
    int threatScore = 0;

    // Check if any opponent piece can capture our piece
    for (int threatRow = 0; threatRow < 8; threatRow++) {
        for (int threatCol = 0; threatCol < 8; threatCol++) {
            std::shared_ptr<Piece> attackerPiece = m_board.getPieceAt(threatRow, threatCol);
            if (attackerPiece && attackerPiece->getIsWhite() != isWhite) {
                std::string attackerSource = coordinatesToNotation(threatRow, threatCol);
                std::string targetDest = coordinatesToNotation(row, col);

                int attackCode = m_board.validateMove(attackerSource, targetDest);
                if (attackCode == 41 || attackCode == 42) {
                    // Our piece is threatened
                    int attackerValue = getPieceValue(attackerPiece->getSymbol());

                    // If threatened by a weaker piece, reduce score
                    if (attackerValue < pieceValue) {
                        threatScore -= (pieceValue - attackerValue) * 5;
                    }
                }
            }
        }
    }

    return threatScore;
}

int MoveRecommender::evaluatePosition(const ChessMove& move) {
    int score = 0;

    // Get move code (if it causes check)
    int moveCode = m_board.validateMove(move.sourcePos, move.destPos);

    // Evaluate capture value
    score += evaluateCapture(move);

    // Evaluate check value
    score += evaluateCheck(moveCode);

    // Evaluate threats
    auto [srcRow, srcCol] = m_board.notationToCoordinates(move.sourcePos);
    auto [destRow, destCol] = m_board.notationToCoordinates(move.destPos);

    std::shared_ptr<Piece> srcPiece = m_board.getPieceAt(srcRow, srcCol);
    if (!srcPiece) return score;

    // Store current state to make temporary move
    bool isWhite = srcPiece->getIsWhite();
    int pieceValue = getPieceValue(srcPiece->getSymbol());

    // Make temporary move to evaluate threats
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        return score + evaluateThreat(destRow, destCol, isWhite, pieceValue);
        });
}

int MoveRecommender::makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc) {
    // Save the complete board state
    BoardState savedState = m_board.saveState();

    // Execute the move
    m_board.makeMove(move.sourcePos, move.destPos);

    // Evaluate position
    int result = evaluationFunc();

    // Restore the board to its previous state
    m_board.restoreState(savedState);

    return result;
}

int MoveRecommender::evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer) {
    // Base case: reached maximum depth or leaf node
    if (depth == 0) {
        return evaluatePosition(move);
    }

    // Make the move and generate opponent's responses
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        // Generate moves for the opposite side
        std::vector<ChessMove> opponentMoves = generateValidMoves(!move.isWhite);

        // If no valid moves for opponent, it's either checkmate or stalemate
        if (opponentMoves.empty()) {
            return isMaximizingPlayer ? 1000 : -1000; // Large value for checkmate
        }

        int bestScore;
        if (isMaximizingPlayer) {
            bestScore = INT_MIN;
            for (const auto& opponentMove : opponentMoves) {
                int score = evaluateMove(opponentMove, depth - 1, false);
                bestScore = std::max(bestScore, score);

                // Early termination for efficiency (optional alpha-beta pruning concept)
                if (bestScore > 500) break;
            }
        }
        else {
            bestScore = INT_MAX;
            for (const auto& opponentMove : opponentMoves) {
                int score = evaluateMove(opponentMove, depth - 1, true);
                bestScore = std::min(bestScore, score);

                // Early termination for efficiency (optional alpha-beta pruning concept)
                if (bestScore < -500) break;
            }
        }

        return bestScore;
        });
}

std::vector<ChessMove> MoveRecommender::recommendMoves(int topN) {
    // Check if we need to refresh the move queues (if they're empty or too few moves)
    if ((m_isWhiteTurn && m_whiteMoveQueue.size() < topN) ||
        (!m_isWhiteTurn && m_blackMoveQueue.size() < topN)) {
        refreshMoveQueues(topN);
    }

    // Extract top N moves from the appropriate queue
    std::vector<ChessMove> recommendations;

    // Create a temporary queue to avoid destroying the original queue
    PriorityQueue<ChessMove, ChessMoveComparator> tempQueue =
        m_isWhiteTurn ? m_whiteMoveQueue : m_blackMoveQueue;

    while (!tempQueue.isEmpty() && recommendations.size() < topN) {
        recommendations.push_back(tempQueue.poll());
    }

    return recommendations;
}

void MoveRecommender::printRecommendations(const std::vector<ChessMove>& recommendations) const {
    std::cout << "Recommended moves for " << (m_isWhiteTurn ? "White" : "Black") << ":" << std::endl;
    int rank = 1;
    for (const auto& move : recommendations) {
        std::cout << rank << ". " << move.toString() << std::endl;
        rank++;
    }
}