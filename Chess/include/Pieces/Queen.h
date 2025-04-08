#pragma once

#include "Piece.h"
#include <memory>
#include <vector>
#include <cmath>

class Queen : public Piece {
public:
    Queen(bool isWhite, int row, int col);
    bool isValidMove(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    bool isPathClear(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    char getSymbol() const override;
};