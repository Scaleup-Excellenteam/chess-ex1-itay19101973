#include "Pieces/King.h"

bool King::isValidMove(int destX, int destY, 
	const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const{
	
    // King can move one square in any direction
    int dx = std::abs(destX - this->getRow());
    int dy = std::abs(destY - this->getCol());

    return (dx <= 1 && dy <= 1) && !(dx == 0 && dy == 0);
}


// no path to check ( checking if square is empty is implemented in board class)
bool King::isPathClear(int destX, int destY, 
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const{

    return true;
}

char King::getSymbol() const{
    return this->getIsWhite() ? 'K' : 'k';
}


// register the king to the factory
bool King::isRegistered = PieceFactory::registerPiece('K', [](bool isWhite, int x, int y) {
    return std::make_shared<King>(isWhite, x, y);
    });