#pragma once

#include "PieceFactory/PieceFactory.h"
#include "Piece.h"
#include <memory>
#include <vector>
#include <cmath>

/**
 * @class Bishop
 * Represents a Bishop piece in a chess game.
 */

class Bishop : public Piece {
public:
    Bishop(bool isWhite, int row, int col);
    bool isValidMove(int destRow, int destCol, 
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;

    bool isPathClear(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    char getSymbol() const override;
private:
    static bool m_isRegistered;
};