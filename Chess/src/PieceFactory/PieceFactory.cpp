#include "PieceFactory/PieceFactory.h"

std::shared_ptr<Piece> PieceFactory::createPiece(char symbol, int x, int y) {

    bool isWhite = std::isupper(symbol);
    char upperSymbol = std::toupper(symbol);

    switch (upperSymbol) {
    case 'R': return std::make_shared<Rook>(isWhite, x, y);
    //case 'N': return std::make_shared<Knight>(isWhite, x, y);
    case 'B': return std::make_shared<Bishop>(isWhite, x, y);
    case 'K': return std::make_shared<King>(isWhite, x, y);
    case 'Q': return std::make_shared<Queen>(isWhite, x, y);
    //case 'P': return std::make_shared<Pawn>(isWhite, x, y);
    default: return nullptr; // Empty square
    }
}


