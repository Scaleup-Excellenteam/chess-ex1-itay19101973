#pragma once

#include "Piece.h"
#include <cmath>

class Rook : public Piece {
public:
    Rook(bool isWhite, int x, int y);

    bool isValidMove(int destX, int destY, 
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override; 

    char getSymbol() const override; 


private:
    bool isPathClear(int destX, int destY, 
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;

};