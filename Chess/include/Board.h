#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Pieces/Piece.h"
#include "PieceFactory/PieceFactory.h"
#include "Pieces/King.h"


class Board {

public:
    Board(const std::string& initialBoard);
    std::pair<int, int> notationToCoordinates(std::string notation); 
    int validateMove(const std::string& source, const std::string& dest);
    void makeMove(const std::string& source, const std::string& dest); 
    bool isKingInCheck(bool isWhiteKing); 
    bool getIsWhiteTurn() const; 

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    bool isWhiteTurn;
    int whiteKingRow, whiteKingCol;
    int blackKingRow, blackKingCol;

    int validateBasicRules(int srcRow, int srcCol, int  destRow, int destCol) const;
    int validatePieceMovement(int srcRow, int srcCol, int destRow, int destCol)const;
    bool isKingMoving(std::shared_ptr<Piece> piece)const;
    void updateKingPos(std::shared_ptr<Piece> piece, int& oldKingRow, int& oldKingCol, const int& destRow, const int& destCol);
    void restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
        int srcRow, int srcCol, int destRow, int destCol, bool isKingMoving);
};



