#pragma once
#include "Board.h"
#include "PriorityQueue.h"
#include <string>
#include <utility>
#include <vector>
#include <iostream>

// Structure to represent a chess move with its score
struct ChessMove {
    std::string sourcePos;
    std::string destPos;
    int score;

    ChessMove(const std::string& source, const std::string& dest, int moveScore = 0)
        : sourcePos(source), destPos(dest), score(moveScore) {
    }

    // For displaying a move
    std::string toString() const {
        return sourcePos + destPos + " (Score: " + std::to_string(score) + ")";
    }
};

// Comparator for ChessMove objects based on their score
struct ChessMoveComparator {
    int operator()(const ChessMove& a, const ChessMove& b) const {
        return a.score - b.score; // Higher score is better
    }
};


class MoveRecommender {
public:
    MoveRecommender(Board& board, int maxDepth = 2);

    // Generate recommendations for best moves
    std::vector<ChessMove> recommendMoves(int topN = 3);

    // Print recommended moves to console
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;

private:
    Board& m_board;
    int m_maxDepth;

    // Chess notation conversion
    std::string coordinatesToNotation(int row, int col) const;

    // Move generation and validation
    std::vector<ChessMove> generateValidMoves() const;

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