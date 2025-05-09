#pragma once

#include "Piece.h"
#include "PieceFactory/PieceFactory.h"


class Pawn : public Piece {
public:
    Pawn(bool isWhite, int row, int col);
    virtual bool isValidMove(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    virtual bool isPathClear(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const override;
    virtual char getSymbol() const override;

private:
    static bool isRegistered;

    
    bool isValidForwardMove(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const;
    bool isValidDoubleMove(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const;
    bool isValidDiagonalCapture(int destRow, int destCol,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const;
    int getForwardDirection() const;
    bool isAtStartingPosition() const;
};