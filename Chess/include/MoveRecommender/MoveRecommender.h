#pragma once
#include "Board.h"
#include "PriorityQueue.h"
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <memory>

// Structure to represent a chess move with its score
struct ChessMove {
    std::string sourcePos;
    std::string destPos;
    int score;
    bool isWhite;  // Added to track which side this move is for

    ChessMove(const std::string& source, const std::string& dest, bool forWhite = true, int moveScore = 0)
        : sourcePos(source), destPos(dest), score(moveScore), isWhite(forWhite) {
    }

    // For displaying a move
    std::string toString() const {
        return sourcePos + destPos + " (Score: " + std::to_string(score) + ")";
    }

    // Equality operator to check if a move is the same
    bool operator==(const ChessMove& other) const {
        return sourcePos == other.sourcePos && destPos == other.destPos;
    }
};

// Comparator for ChessMove objects based on their score
struct ChessMoveComparator {
    int operator()(const ChessMove& a, const ChessMove& b) const {
        return a.score - b.score; // Now correctly prioritizes higher scores
    }
};

class MoveRecommender {
public:
    MoveRecommender(Board& board, int maxDepth);

    // Recommend top N moves for current player
    std::vector<ChessMove> recommendMoves(int topN);

    // Update cached moves when a move is played
    void updateCachedMoves(const std::string& source, const std::string& dest);

    // Print recommendations to console (for debugging)
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;

private:
    Board& m_board;
    int m_maxDepth;
    bool m_isWhiteTurn;

    // Priority queues for caching move evaluations
    PriorityQueue<ChessMove, ChessMoveComparator> m_whiteMoveQueue;
    PriorityQueue<ChessMove, ChessMoveComparator> m_blackMoveQueue;

    // Helper methods
    std::string coordinatesToNotation(int row, int col) const;
    bool isMoveStillValid(const ChessMove& move) const;
    void refreshMoveQueues(int topN);

    // Evaluation methods
    int evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer);
    int evaluatePosition(const ChessMove& move);
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);

    // Component evaluation methods
    int getPieceValue(char pieceSymbol) const;
    int evaluateCapture(const ChessMove& move) const;
    int evaluateCheck(int moveCode) const;
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);

    // New evaluation methods
    int evaluateCenterControl(int row, int col) const;  // Evaluate center control
    int evaluateKingMove(const ChessMove& move) const;  // Penalize king moves (except castling)
};