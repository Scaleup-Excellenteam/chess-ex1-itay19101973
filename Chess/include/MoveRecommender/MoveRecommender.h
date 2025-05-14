#ifndef MOVERECOMMENDER_H
#define MOVERECOMMENDER_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include "Board/Board.h"
#include "ChessMove.h"
#include "PriorityQueue.h"

/**
 * @brief Class for recommending chess moves using board evaluation
 */
class MoveRecommender {
private:
    Board& m_board;                       // Reference to the chess board
    int m_maxDepth;                       // Maximum search depth for move evaluation
    bool m_isWhiteTurn;                   // Flag to track whose turn it is
    PriorityQueue<ChessMove, ChessMoveComparator> m_moveQueue;  // Single priority queue for moves

    // Helper functions for move evaluation
    std::string coordinatesToNotation(int row, int col) const;
    bool isMoveStillValid(const ChessMove& move) const;
    void refreshMoveQueue(int topN);
    int getPieceValue(char pieceSymbol) const;
    int evaluateCheck(int moveCode) const;
    int evaluateCenterControl(int row, int col) const;
    int evaluateKingMove(const ChessMove& move) const;
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);
    int evaluatePosition(const ChessMove& move);
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);
    int evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer);

public:
    // Constructor
    MoveRecommender(Board& board, int maxDepth);

    // Primary public methods
    std::vector<ChessMove> recommendMoves(int topN);
    void updateCachedMoves(const std::string& source, const std::string& dest);
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;
};

#endif // MOVERECOMMENDER_H