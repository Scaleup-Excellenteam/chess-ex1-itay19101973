#pragma once

#include "Piece.h"
#include "PieceFactory/PieceFactory.h"

class Knight : public Piece {
public:
    Knight(bool isWhite, int row, int col);
    virtual bool isValidMove(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    virtual bool isPathClear(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    virtual char getSymbol() const override;

private:
    static bool isRegistered;

    
    bool isValidLShapeMove(int destRow, int destCol) const;
};