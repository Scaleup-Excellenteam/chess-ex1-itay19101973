#pragma once

#include "Piece.h"
#include <cmath>
#include "PieceFactory/PieceFactory.h"

class King : public Piece {
public:
    King(bool isWhite, int x, int y) : Piece(isWhite, x, y) {}

    bool isValidMove(int destX, int destY,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override; 
       

    bool isPathClear(int destX, int destY,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override; 

    char getSymbol() const override;
private:
    static bool isRegistered;
};