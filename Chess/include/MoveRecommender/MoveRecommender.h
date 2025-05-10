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

// Class for recommending chess moves
class MoveRecommender {
private:
    Board& m_board;
    int m_maxDepth;

    // Convert board coordinates to chess notation
    std::string coordinatesToNotation(int row, int col) const;

    // Generate all possible valid moves for current player
    std::vector<ChessMove> generateValidMoves() const;

    // Evaluate a move's score at the given depth
    int evaluateMove(const ChessMove& move, int depth, bool isMaximizingPlayer);

    // Evaluate the immediate value of a position
    int evaluatePosition(const ChessMove& move);

    // Calculate piece value
    int getPieceValue(char pieceSymbol) const;

    // Check if piece is threatened by a weaker piece
    int evaluateThreat(int row, int col, bool isWhite, int pieceValue);

    // Evaluate capture value
    int evaluateCapture(const ChessMove& move) const;

    // Evaluate check value
    int evaluateCheck(int moveCode) const;

    // Make a temporary move and restore afterward
    int makeTemporaryMoveAndEvaluate(const ChessMove& move, std::function<int()> evaluationFunc);

public:
    MoveRecommender(Board& board, int maxDepth = 2);

    // Recommend top N moves
    std::vector<ChessMove> recommendMoves(int topN = 5);

    // Output recommendations to console
    void printRecommendations(const std::vector<ChessMove>& recommendations) const;
};