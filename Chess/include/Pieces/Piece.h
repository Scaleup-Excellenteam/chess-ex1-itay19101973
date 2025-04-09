#pragma once

#include <string>
#include <vector>
#include <memory>
/**
 * @class Piece
 * Abstract base class representing a chess piece.
 */
class Piece {


public:
    Piece(bool isWhite, int row, int col);
    virtual ~Piece() = default;
    virtual bool isValidMove(int destRow, int destCol, 
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0; 
    virtual bool isPathClear(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0;

    // Getters
    bool getIsWhite() const;
    int getRow() const;
    int getCol() const;

    // Setters
    void setPosition(int newRow, int newCol);

    virtual char getSymbol() const = 0;

private:
    bool m_isWhite;
    int m_Row, m_Col;
};