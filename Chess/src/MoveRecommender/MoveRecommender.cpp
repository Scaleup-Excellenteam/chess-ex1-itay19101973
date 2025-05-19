#include "MoveRecommender/MoveRecommender.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <list>

/**
 * @brief Constructor for the MoveRecommender class.
 */
MoveRecommender::MoveRecommender(Board& board, int maxDepth)
    : m_board(board), m_maxDepth(maxDepth), m_isWhiteTurn(true),
    m_moveQueue(ChessUtils::MAX_QUEUE_SIZE) {
}

/**
 * @brief Converts board coordinates to chess notation (e.g., A1, H8).
 */
std::string MoveRecommender::coordinatesToNotation(int row, int col) const {
    char file = 'A' + row;
    char rank = '1' + col;
    return std::string(1, file) + std::string(1, rank);
}

/**
 * @brief Checks if a move is still valid on the current board.
 */
bool MoveRecommender::isMoveStillValid(const ChessMove& move) const {
    int moveCode = m_board.validateMove(move.getSourcePos(), move.getDestPos());
    return (moveCode == ChessUtils::VALID_MOVE || moveCode == ChessUtils::VALID_MOVE_CHECK);
}

/**
 * @brief Gets the numerical value of a chess piece.
 */
int MoveRecommender::getPieceValue(char pieceSymbol) const {
    switch (tolower(pieceSymbol)) {
    case static_cast<char>(ChessUtils::PieceType::PAWN):   return ChessUtils::PAWN_VALUE;
    case static_cast<char>(ChessUtils::PieceType::KNIGHT): return ChessUtils::KNIGHT_VALUE;
    case static_cast<char>(ChessUtils::PieceType::BISHOP): return ChessUtils::BISHOP_VALUE;
    case static_cast<char>(ChessUtils::PieceType::ROOK):   return ChessUtils::ROOK_VALUE;
    case static_cast<char>(ChessUtils::PieceType::QUEEN):  return ChessUtils::QUEEN_VALUE;
    case static_cast<char>(ChessUtils::PieceType::KING):   return ChessUtils::KING_VALUE;
    default: return 0;
    }
}

/**
 * @brief Evaluates all possible moves and fills the priority queue.
 */
void MoveRecommender::refreshMoveQueue() {
    m_moveQueue = PriorityQueue<ChessMove, ChessMoveComparator>(ChessUtils::MAX_QUEUE_SIZE);

    // Iterate through all board positions
    for (int row = 0; row < ChessUtils::BOARD_SIZE; row++) {
        for (int col = 0; col < ChessUtils::BOARD_SIZE; col++) {
            std::shared_ptr<Piece> piece = m_board.getPieceAt(row, col);

            // Skip if no piece or wrong color
            if (!piece || piece->getIsWhite() != m_isWhiteTurn) {
                continue;
            }

            // Check all possible destinations for this piece
            evaluateMovesForPiece(row, col);
        }
    }
}

/**
 * @brief Evaluates all possible moves for a piece at given position.
 */
void MoveRecommender::evaluateMovesForPiece(int srcRow, int srcCol) {
    std::string source = coordinatesToNotation(srcRow, srcCol);

    for (int destRow = 0; destRow < ChessUtils::BOARD_SIZE; destRow++) {
        for (int destCol = 0; destCol < ChessUtils::BOARD_SIZE; destCol++) {
            std::string dest = coordinatesToNotation(destRow, destCol);

            int moveCode = m_board.validateMove(source, dest);
            if (moveCode == ChessUtils::VALID_MOVE || moveCode == ChessUtils::VALID_MOVE_CHECK) {
                try {
                    ChessMove move(source, dest, m_isWhiteTurn);
                    int score = minimax(move, m_maxDepth, true);
                    move.setScore(score);

                    if (score != 0) {
                        m_moveQueue.push(move);
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Error evaluating move " << source << dest
                        << ": " << e.what() << std::endl;
                }
            }
        }
    }
}

/**
 * @brief Simplified minimax algorithm for move evaluation.
 *
 * This is the core recursive function that:
 * 1. Evaluates the current position
 * 2. If we haven't reached max depth, looks ahead at opponent responses
 * 3. Returns the best score assuming both players play optimally
 */
int MoveRecommender::minimax(const ChessMove& move, int depth, bool isMaximizing) {
    // Step 1: Get the immediate score for this move
    int currentScore = evaluatePosition(move);

    // Step 2: If we've reached the bottom or this is a leaf, return the score
    if (depth == 0) {
        return currentScore;
    }

    // Step 3: Look ahead - make the move temporarily and see what opponent can do
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        int bestScore = isMaximizing ? INT_MIN : INT_MAX;
        bool foundMove = false;

        // Check all possible opponent responses
        for (int row = 0; row < ChessUtils::BOARD_SIZE; row++) {
            for (int col = 0; col < ChessUtils::BOARD_SIZE; col++) {
                std::shared_ptr<Piece> piece = m_board.getPieceAt(row, col);

                // Skip if not opponent's piece
                if (!piece || piece->getIsWhite() == move.getIsWhite()) {
                    continue;
                }

                // Try all destinations for this opponent piece
                for (int destRow = 0; destRow < ChessUtils::BOARD_SIZE; destRow++) {
                    for (int destCol = 0; destCol < ChessUtils::BOARD_SIZE; destCol++) {
                        std::string source = coordinatesToNotation(row, col);
                        std::string dest = coordinatesToNotation(destRow, destCol);

                        int moveCode = m_board.validateMove(source, dest);
                        if (moveCode == ChessUtils::VALID_MOVE || moveCode == ChessUtils::VALID_MOVE_CHECK) {
                            foundMove = true;
                            ChessMove opponentMove(source, dest, !move.getIsWhite());
                            int opponentScore = minimax(opponentMove, depth - 1, !isMaximizing);

                            // Update best score based on who's playing
                            if (isMaximizing) {
                                bestScore = std::max(bestScore, opponentScore);
                            }
                            else {
                                bestScore = std::min(bestScore, opponentScore);
                            }

                            // Early exit if we found a really good/bad move
                            if ((isMaximizing && bestScore > ChessUtils::ALPHA_BETA_CUTOFF) ||
                                (!isMaximizing && bestScore < -ChessUtils::ALPHA_BETA_CUTOFF)) {
                                return currentScore + bestScore / depth;
                            }
                        }
                    }
                }
            }
        }

        // If opponent has no moves, it's checkmate
        if (!foundMove) {
            return isMaximizing ? ChessUtils::CHECKMATE_SCORE : -ChessUtils::CHECKMATE_SCORE;
        }

        // Combine current move score with best opponent response
        return currentScore + bestScore / depth;
        });
}

/**
 * @brief Evaluates a position based on multiple factors.
 */
int MoveRecommender::evaluatePosition(const ChessMove& move) {
    auto [srcRow, srcCol] = m_board.notationToCoordinates(move.getSourcePos());
    auto [destRow, destCol] = m_board.notationToCoordinates(move.getDestPos());

    std::shared_ptr<Piece> movingPiece = m_board.getPieceAt(srcRow, srcCol);
    if (!movingPiece) return 0;

    int score = 0;
    int moveCode = m_board.validateMove(move.getSourcePos(), move.getDestPos());

    // 1. Capture bonus
    std::shared_ptr<Piece> capturedPiece = m_board.getPieceAt(destRow, destCol);
    if (capturedPiece && capturedPiece->getIsWhite() != movingPiece->getIsWhite()) {
        score += getPieceValue(capturedPiece->getSymbol()) * ChessUtils::CAPTURE_MULTIPLIER;
    }

    // 2. Check bonus
    if (moveCode == ChessUtils::VALID_MOVE_CHECK) {
        score += ChessUtils::CHECK_BONUS;
    }

    // 3. Center control bonus
    score += evaluateCenterControl(destRow, destCol);

    // 4. King move penalty (generally avoid moving king unless necessary)
    if (tolower(movingPiece->getSymbol()) == static_cast<char>(ChessUtils::PieceType::KING)) {
        score += ChessUtils::KING_MOVE_PENALTY;
    }

    // 5. Add small randomness to vary play
    score += rand() % ChessUtils::RANDOMNESS_RANGE;

    // 6. Evaluate threats after the move
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        int pieceValue = getPieceValue(movingPiece->getSymbol());
        return score + evaluateThreat(destRow, destCol, movingPiece->getIsWhite(), pieceValue);
        });
}

/**
 * @brief Evaluates center control bonus.
 */
int MoveRecommender::evaluateCenterControl(int row, int col) const {
    // Check inner center squares
    for (int i = 0; i < 4; i++) {
        if (row == ChessUtils::CENTER_SQUARES_INNER[i][0] &&
            col == ChessUtils::CENTER_SQUARES_INNER[i][1]) {
            return ChessUtils::CENTER_BONUS_INNER;
        }
    }

    // Check outer center squares
    for (int i = 0; i < 12; i++) {
        if (row == ChessUtils::CENTER_SQUARES_OUTER[i][0] &&
            col == ChessUtils::CENTER_SQUARES_OUTER[i][1]) {
            return ChessUtils::CENTER_BONUS_OUTER;
        }
    }

    return 0;
}

/**
 * @brief Evaluates threats to a piece at given position.
 */
int MoveRecommender::evaluateThreat(int row, int col, bool isWhite, int pieceValue) {
    int threatScore = 0;

    // Check if any opponent piece can capture our piece
    for (int attackerRow = 0; attackerRow < ChessUtils::BOARD_SIZE; attackerRow++) {
        for (int attackerCol = 0; attackerCol < ChessUtils::BOARD_SIZE; attackerCol++) {
            std::shared_ptr<Piece> attacker = m_board.getPieceAt(attackerRow, attackerCol);

            if (attacker && attacker->getIsWhite() != isWhite) {
                std::string source = coordinatesToNotation(attackerRow, attackerCol);
                std::string dest = coordinatesToNotation(row, col);

                int moveCode = m_board.validateMove(source, dest);
                if (moveCode == ChessUtils::VALID_MOVE || moveCode == ChessUtils::VALID_MOVE_CHECK) {
                    int attackerValue = getPieceValue(attacker->getSymbol());

                    // Heavy penalty if threatened by weaker piece
                    if (attackerValue < pieceValue) {
                        threatScore -= (pieceValue - attackerValue) * ChessUtils::THREAT_MULTIPLIER;
                    }
                    else {
                        threatScore -= ChessUtils::THREAT_PENALTY;
                    }
                }
            }
        }
    }

    return threatScore;
}

/**
 * @brief Makes a temporary move, evaluates, then restores board.
 */
int MoveRecommender::makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc) {
    BoardState savedState = m_board.saveState();
    m_board.makeMove(move.getSourcePos(), move.getDestPos());
    int result = evaluationFunc();
    m_board.restoreState(savedState);
    return result;
}

/**
 * @brief Main function to get move recommendations.
 */
void MoveRecommender::recommendMoves() {
    refreshMoveQueue();



    m_isWhiteTurn = !m_isWhiteTurn;
}

/**
 * @brief Prints recommended moves.
 */
void MoveRecommender::printRecommendations() const {
    std::cout << m_moveQueue;
}