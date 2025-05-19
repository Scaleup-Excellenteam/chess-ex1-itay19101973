#include "MoveRecommender/MoveRecommender.h"


/**
 * @brief Constructor for the MoveRecommender class.
 *
 * @param board Reference to the chess board object
 * @param maxDepth Maximum search depth for move evaluation
 */
MoveRecommender::MoveRecommender(Board& board, int maxDepth)
    : m_board(board), m_maxDepth(maxDepth), m_isWhiteTurn(true),
    m_moveQueue(5) {  // Initialize single queue with max size 5
}
//==========================================================================================
/**
 * @brief Converts board coordinates to chess notation (e.g., A1, H8).
 *
 * @param row The board row (0-7)
 * @param col The board column (0-7)
 * @return std::string The chess notation for the position
 */
std::string MoveRecommender::coordinatesToNotation(int row, int col) const {
    char file = 'A' + row;  // Convert row to file (A-H)
    char rank = '1' + col;  // Convert col to rank (1-8)
    return std::string(1, file) + std::string(1, rank);
}
/**
 * @brief Checks if a previously calculated move is still valid on the current board.
 *
 * @param move The chess move to validate
 * @return bool True if the move is still valid, false otherwise
 */
bool MoveRecommender::isMoveStillValid(const ChessMove& move) const {
    int moveCode = m_board.validateMove(move.getSourcePos(), move.getDestPos());
    return (moveCode == 41 || moveCode == 42);
}
/**
 * @brief Refreshes the move priority queue by recalculating valid moves and their scores.
 *
 * @param topN The number of top moves to maintain in the queue
 */
void MoveRecommender::refreshMoveQueue() {
    // Clear the queue and rebuild it for the current player
    m_moveQueue = PriorityQueue<ChessMove, ChessMoveComparator>(5);  // Max size 5

    
    for (int srcRow = 0; srcRow < 8; srcRow++) {
        for (int srcCol = 0; srcCol < 8; srcCol++) {
            // Check if there's a piece at this position
            std::shared_ptr<Piece> piece = m_board.getPieceAt(srcRow, srcCol);

            // Skip if no piece or if piece color doesn't match the current side
            if (!piece || piece->getIsWhite() != m_isWhiteTurn) {
                continue;
            }

            std::string source = coordinatesToNotation(srcRow, srcCol);

            // Check all possible destinations
            for (int destRow = 0; destRow < 8; destRow++) {
                for (int destCol = 0; destCol < 8; destCol++) {
                    std::string dest = coordinatesToNotation(destRow, destCol);

                    // Check if the move is valid
                    int moveCode = m_board.validateMove(source, dest);
                    if (moveCode == 41 || moveCode == 42) {
                        // Valid move - evaluate it directly
                        try {
                            ChessMove move(source, dest, m_isWhiteTurn);
                            int score = evaluateMove(move, m_maxDepth, true);
                            move.setScore(score);

                            // Add to queue if score is meaningful
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
    }

    // Debug output to confirm scores are being calculated //TODO
    std::cout << "Refreshed move queue for " << (m_isWhiteTurn ? "White" : "Black") << std::endl;
    const auto& currentQueue = m_moveQueue.getList();
    for (const auto& move : currentQueue) {
        std::cout << "  " << move.toString() << std::endl;
    }
}
/**
 * @brief Gets the numerical value of a chess piece based on its symbol.
 *
 * @param pieceSymbol The character symbol representing the piece
 * @return int The numerical value of the piece
 */
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
/**
 * @brief Evaluates the score for a move that puts the opponent in check.
 *
 * @param moveCode The validation code returned by the board's validateMove method
 * @return int The score bonus for putting the opponent in check
 */
int MoveRecommender::evaluateCheck(int moveCode) const {
    // If this move puts opponent in check, add significant points (big bonus)
    return (moveCode == 41) ? 50 : 0;
}
/**
 * @brief Evaluates the score bonus for controlling center squares.
 *
 * @param row The destination row
 * @param col The destination column
 * @return int The score bonus based on proximity to the center
 */
int MoveRecommender::evaluateCenterControl(int row, int col) const {
    // Evaluate center control with varying weights based on distance from center
    const int CENTER_SQUARES[16][2] = {
        {3, 3}, {3, 4}, {4, 3}, {4, 4},         // Inner center (strongest bonus)
        {2, 2}, {2, 3}, {2, 4}, {2, 5},         // Extended center (medium bonus)
        {3, 2}, {4, 2}, {5, 2}, {5, 3},
        {5, 4}, {5, 5}, {3, 5}, {4, 5}          // Outer center region (small bonus)
    };

    // Inner center squares (D4, D5, E4, E5)
    for (int i = 0; i < 4; i++) {
        if (row == CENTER_SQUARES[i][0] && col == CENTER_SQUARES[i][1]) {
            return 15; // Strong bonus for controlling the inner center
        }
    }

    // Extended center squares
    for (int i = 4; i < 16; i++) {
        if (row == CENTER_SQUARES[i][0] && col == CENTER_SQUARES[i][1]) {
            return 8; // Medium bonus for extended center control
        }
    }

    return 0; // No center control bonus
}
/**
 * @brief Evaluates the score adjustments for king moves, with special handling for castling.
 *
 * @param move The chess move to evaluate
 * @return int The score adjustment for the king move
 */
int MoveRecommender::evaluateKingMove(const ChessMove& move) const {
    auto [srcRow, srcCol] = m_board.notationToCoordinates(move.getSourcePos());
    std::shared_ptr<Piece> piece = m_board.getPieceAt(srcRow, srcCol);

    // Check if it's a king move
    if (piece && tolower(piece->getSymbol()) == 'k') {

        return -15;
    }

    return 0; // Not a king move
}
/**
 * @brief Evaluates threats against a piece at a specific position.
 *
 * @param row The row of the piece to evaluate
 * @param col The column of the piece to evaluate
 * @param isWhite Whether the piece is white or black
 * @param pieceValue The value of the piece
 * @return int The score penalty based on threats to the piece
 */
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

                    // If threatened by a weaker piece, significantly reduce score (major penalty)
                    if (attackerValue < pieceValue) {
                        threatScore -= (pieceValue - attackerValue) * 25; // Increased penalty multiplier
                    }
                    // Also add a base penalty for any threat
                    else {
                        threatScore -= 10;
                    }
                }
            }
        }
    }

    return threatScore;
}
/**
 * @brief Evaluates the positional score of a move based on multiple factors.
 *
 * @param move The chess move to evaluate
 * @return int The combined positional score of the move
 */
int MoveRecommender::evaluatePosition(const ChessMove& move) {
    int score = 0;

    // Get source and destination coordinates
    auto [srcRow, srcCol] = m_board.notationToCoordinates(move.getSourcePos());
    auto [destRow, destCol] = m_board.notationToCoordinates(move.getDestPos());

    // Get source piece information
    std::shared_ptr<Piece> srcPiece = m_board.getPieceAt(srcRow, srcCol);
    if (!srcPiece) return score;

    // Get move code (if it causes check)
    int moveCode = m_board.validateMove(move.getSourcePos(), move.getDestPos());

    // 1. Evaluate captures (medium bonus)
    std::shared_ptr<Piece> destPiece = m_board.getPieceAt(destRow, destCol);
    if (destPiece && destPiece->getIsWhite() != srcPiece->getIsWhite()) {
        int captureValue = getPieceValue(destPiece->getSymbol());
        int attackerValue = getPieceValue(srcPiece->getSymbol());

        // Base capture value (medium bonus)
        score += captureValue * 20;

        // Extra bonus for capturing with lower value pieces (tactical advantage)
        if (attackerValue < captureValue) {
            score += (captureValue - attackerValue) * 25; // Higher bonus for favorable trades
        }
    }

    // 2. Evaluate check (big bonus)
    score += evaluateCheck(moveCode);

    // 3. Evaluate center control (moderate bonus)
    score += evaluateCenterControl(destRow, destCol);

    // 4. Penalize or reward king moves
    score += evaluateKingMove(move);

    // 5. Add a small random component to vary recommendations
    score += rand() % 3;  // Add 0-2 points of randomness

    // Store current state to make temporary move
    bool isWhite = srcPiece->getIsWhite();
    int pieceValue = getPieceValue(srcPiece->getSymbol());

    // 6. Make temporary move to evaluate threats (significant penalty)
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        // Threat evaluation is now a major factor in scoring
        int threatEvaluation = evaluateThreat(destRow, destCol, isWhite, pieceValue);

        // Weight threat evaluation heavily as it's one of the main priorities
        return score + threatEvaluation;
        });
}
/**
 * @brief Makes a temporary move on the board, evaluates it, and restores the board state.
 *
 * @param move The chess move to make temporarily
 * @param evaluationFunc A function that performs the evaluation
 * @return int The evaluation result
 */
int MoveRecommender::makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc) {
    // Save the complete board state
    BoardState savedState = m_board.saveState();

    // Execute the move
    m_board.makeMove(move.getSourcePos(), move.getDestPos());

    // Evaluate position
    int result = evaluationFunc();

    // Restore the board to its previous state
    m_board.restoreState(savedState);

    return result;
}
/**
 * @brief Recursively evaluates a move using minimax algorithm with a specified depth.
 *
 * @param move The chess move to evaluate
 * @param depth The current search depth
 * @param isMaximizingPlayer Whether the current player is maximizing (true) or minimizing (false)
 * @return int The evaluation score of the move
 */
int MoveRecommender::evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer) {
    // Base score from direct position evaluation
    int baseScore = evaluatePosition(move);

    // Base case: reached maximum depth or leaf node
    if (depth == 0) {
        return baseScore;
    }

    // Make the move and evaluate opponent responses
    return makeTemporaryMoveAndEvaluate(move, [&]() {
        // Create variables to track best response score
        int bestScore = isMaximizingPlayer ? INT_MIN : INT_MAX;
        bool foundValidMove = false;

        // Temporary board state changes have been made - now check opponent responses
        for (int srcRow = 0; srcRow < 8; srcRow++) {
            for (int srcCol = 0; srcCol < 8; srcCol++) {
                // Get piece at this position
                std::shared_ptr<Piece> piece = m_board.getPieceAt(srcRow, srcCol);

                // Skip if no piece or if piece color doesn't match the opposite side
                if (!piece || piece->getIsWhite() == move.getIsWhite()) {
                    continue;
                }

                std::string source = coordinatesToNotation(srcRow, srcCol);

                // Check all possible destinations
                for (int destRow = 0; destRow < 8; destRow++) {
                    for (int destCol = 0; destCol < 8; destCol++) {
                        std::string dest = coordinatesToNotation(destRow, destCol);

                        // Check if the move is valid
                        int moveCode = m_board.validateMove(source, dest);
                        if (moveCode == 41 || moveCode == 42) {
                            // Found a valid response - evaluate it
                            ChessMove responseMove(source, dest, !move.getIsWhite());
                            int score = evaluateMove(responseMove, depth - 1, !isMaximizingPlayer);
                            foundValidMove = true;

                            // Update best score based on minimax principle
                            if (isMaximizingPlayer) {
                                bestScore = std::max(bestScore, score);
                                // Early termination for efficiency
                                if (bestScore > 500) break;
                            }
                            else {
                                bestScore = std::min(bestScore, score);
                                // Early termination for efficiency
                                if (bestScore < -500) break;
                            }
                        }
                    }
                    if ((isMaximizingPlayer && bestScore > 500) ||
                        (!isMaximizingPlayer && bestScore < -500)) {
                        break;
                    }
                }
                if ((isMaximizingPlayer && bestScore > 500) ||
                    (!isMaximizingPlayer && bestScore < -500)) {
                    break;
                }
            }
            if ((isMaximizingPlayer && bestScore > 500) ||
                (!isMaximizingPlayer && bestScore < -500)) {
                break;
            }
        }

        // If no valid moves for opponent, it's either checkmate or stalemate
        if (!foundValidMove) {
            return isMaximizingPlayer ? 1000 : -1000; // Large value for checkmate
        }

        // Return the combined score - base evaluation plus minimax result
        return baseScore + (bestScore / (depth + 1));  // Weight by depth
        });
}
/**
 * @brief Recommends the top N moves for the current player.
 *
 * @param topN The number of top moves to recommend
 * @return std::vector<ChessMove> Vector containing the recommended moves
 */
std::vector<ChessMove> MoveRecommender::recommendMoves() {
    
    
    refreshMoveQueue();
    

    // Extract top N moves from the queue
    std::vector<ChessMove> recommendations;

    // Create a temporary queue to avoid destroying the original queue
    PriorityQueue<ChessMove, ChessMoveComparator> tempQueue = m_moveQueue;

   

    this->m_isWhiteTurn = !this->m_isWhiteTurn;

    return recommendations;
}
/**
 * @brief Prints the recommended moves to the console.
 *
 * @param recommendations Vector of recommended chess moves
 */
void MoveRecommender::printRecommendations() const {
    std::cout << this->m_moveQueue;
}