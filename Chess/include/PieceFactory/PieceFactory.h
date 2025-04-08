#pragma once



#include "Pieces/Piece.h"
#include "Pieces/Rook.h"
#include "Pieces/King.h"
#include "Pieces/Bishop.h"
#include "Pieces/Queen.h"

class PieceFactory {
public:
    static std::shared_ptr<Piece> createPiece(char symbol, int x, int y); 
};