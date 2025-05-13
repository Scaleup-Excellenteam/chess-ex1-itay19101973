#pragma once


#include <string>

class ChessMove {
private:
    std::string m_sourcePos;
    std::string m_destPos;
    int m_score;
    bool m_isWhite;

public:
    // Constructor
    ChessMove(const std::string& source, const std::string& dest, bool forWhite = true, int moveScore = 0);

    // Copy constructor
    ChessMove(const ChessMove& other);

    // Assignment operator
    ChessMove& operator=(const ChessMove& other);

    // Equality operator
    bool operator==(const ChessMove& other) const;

    // Getters
    std::string getSourcePos() const;
    std::string getDestPos() const;
    int getScore() const;
    bool getIsWhite() const;

    // Setters
    void setScore(int score);

    // Utility methods
    std::string toString() const;
};

// Comparator for ChessMove objects based on their score
struct ChessMoveComparator {
    int operator()(const ChessMove& a, const ChessMove& b) const {
        return a.getScore() - b.getScore(); // Prioritizes higher scores
    }
};