#ifndef MOVERECOMMENDER_H
#define MOVERECOMMENDER_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "Board/Board.h"
#include "ChessMove.h"
#include "PriorityQueue.h"
#include "ChessUtils.h"

/**
 * @brief Simplified chess move recommender using minimax algorithm
 */
class MoveRecommender {
private:
    Board& m_board;
    int m_maxDepth;
    bool m_isWhiteTurn;
    PriorityQueue<ChessMove, ChessMoveComparator> m_moveQueue;

    // Core helper functions
    std::string coordinatesToNotation(int row, int col) const;
    bool isMoveStillValid(const ChessMove& move) const;
    int getPieceValue(char pieceSymbol) const;

    // Move generation and evaluation
    void refreshMoveQueue();
    void evaluateMovesForPiece(int srcRow, int srcCol);

    // Simplified minimax algorithm
    int minimax(const ChessMove& move, int depth, bool isMaximizing);

    // Position evaluation functions
    int evaluatePosition(const ChessMove& move);
    int evaluateCenterControl(int row, int col) const;
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);

    // Utility function for temporary moves
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);

public:
    // Constructor
    MoveRecommender(Board& board, int maxDepth);

    // Public interface
    std::vector<ChessMove> recommendMoves();
    void printRecommendations() const;
};

#endif // MOVERECOMMENDER_H