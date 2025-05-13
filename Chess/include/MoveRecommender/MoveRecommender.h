#pragma once
#include "Board/Board.h"
#include "ChessMove.h"
#include "PriorityQueue.h"
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <functional>
#include <iostream>
#include <climits>

class MoveRecommender {
private:
    Board& m_board;
    int m_maxDepth;
    bool m_isWhiteTurn;
    PriorityQueue<ChessMove, ChessMoveComparator> m_whiteMoveQueue;
    PriorityQueue<ChessMove, ChessMoveComparator> m_blackMoveQueue;

    // Convert board coordinates to chess notation
    std::string coordinatesToNotation(int row, int col) const;

    // Check if a move is still valid on the current board
    bool isMoveStillValid(const ChessMove& move) const;

    // Refresh the move queues for the current player
    void refreshMoveQueues(int topN);

    // Piece value helper
    int getPieceValue(char pieceSymbol) const;

    // Evaluation helper methods
    int evaluateCapture(const ChessMove& move) const;
    int evaluateCheck(int moveCode) const;
    int evaluateCenterControl(int row, int col) const;
    int evaluateKingMove(const ChessMove& move) const;
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);
    int evaluatePosition(const ChessMove& move);

    // Make a temporary move, evaluate, and restore board state
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);

    // Evaluate a move using minimax approach to the specified depth
    int evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer);

public:
    MoveRecommender(Board& board, int maxDepth = 2);

    // Update cached moves after a move is played
    void updateCachedMoves(const std::string& source, const std::string& dest);

    // Get top N recommended moves for the current player
    std::vector<ChessMove> recommendMoves(int topN = 3);

    // Display recommendations
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;
};