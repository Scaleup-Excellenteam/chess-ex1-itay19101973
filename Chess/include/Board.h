#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Pieces/Piece.h"
#include "PieceFactory/PieceFactory.h"
#include "Pieces/King.h"

/*
* class Board
* =====================
* This class manages the chessboard, piece positions, turn tracking,
* move validation , move execution, and king check detection.
*/
class Board {

public:
    Board(const std::string& initialBoard);
    
    int validateMove(const std::string& source, const std::string& dest);
    void makeMove(const std::string& source, const std::string& dest); 
     
     

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> m_board;
    bool m_isWhiteTurn;
    int m_whiteKingRow, m_whiteKingCol;
    int m_blackKingRow, m_blackKingCol;

    int validateBasicRules(int srcRow, int srcCol, int  destRow, int destCol) const;
    int validatePieceMovement(int srcRow, int srcCol, int destRow, int destCol)const;
    bool isKingMoving(std::shared_ptr<Piece> piece)const;
    void updateKingPos(std::shared_ptr<Piece> piece, int& oldKingRow,
        int& oldKingCol, const int& destRow, const int& destCol);
    void restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
        int srcRow, int srcCol, int destRow, int destCol, bool isKingMoving);
    std::pair<int, int> notationToCoordinates(std::string notation);
    bool isKingInCheck(bool isWhiteKing);
    bool getIsWhiteTurn() const;
};



