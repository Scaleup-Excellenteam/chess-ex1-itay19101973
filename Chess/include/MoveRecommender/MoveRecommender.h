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
    MoveRecommender(Board& board, int maxDepth = 2);

    // Generate recommendations for best moves
    std::vector<ChessMove> recommendMoves(int topN = 3);

    // Print recommended moves to console
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;

    // Update cached moves after player makes a move
    void updateCachedMoves(const std::string& source, const std::string& dest);

private:
    Board& m_board;
    int m_maxDepth;

    // Separate cached priority queues for white and black moves
    PriorityQueue<ChessMove, ChessMoveComparator> m_whiteMoveQueue;
    PriorityQueue<ChessMove, ChessMoveComparator> m_blackMoveQueue;

    // Keep track of whose turn it is
    bool m_isWhiteTurn;

    // Chess notation conversion
    std::string coordinatesToNotation(int row, int col) const;

    // Move generation and validation
    std::vector<ChessMove> generateValidMoves(bool forWhite) const;

    // Validate if a cached move is still valid
    bool isMoveStillValid(const ChessMove& move) const;

    // Refresh the move queues
    void refreshMoveQueues(int topN);

    // Piece valuation
    int getPieceValue(char pieceSymbol) const;

    // Position evaluation functions
    int evaluateCapture(const ChessMove& move) const;
    int evaluateCheck(int moveCode) const;
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);
    int evaluatePosition(const ChessMove& move);
    int evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer);

    // Temporary move handling for evaluation
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);
};