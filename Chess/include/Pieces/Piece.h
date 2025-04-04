#pragma once

#include <string>
#include <vector>
#include <memory>

class Piece {


public:
    Piece(bool isWhite, int x, int y);
    virtual ~Piece() = default;

    // Check if a move is valid for this piece
    virtual bool isValidMove(int destX, int destY, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0;

    // Check if the path to the destination is clear (for pieces like rook, bishop, queen)
    virtual bool isPathClear(int destX, int destY, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0;

    // Getters
    bool getIsWhite() const;
    int getX() const;
    int getY() const;

    // Setters
    void setPosition(int newX, int newY);

    // Get the character representation of the piece
    virtual char getSymbol() const = 0;

private:
    bool m_isWhite;
    int m_x, m_y;
};