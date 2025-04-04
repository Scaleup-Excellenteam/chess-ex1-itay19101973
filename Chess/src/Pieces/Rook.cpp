#include "Pieces/Rook.h"

Rook::Rook(bool isWhite, int x, int y) : Piece(isWhite, x, y) {}

bool Rook::isValidMove(int destX, int destY, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const{
    // Rook can only move horizontally or vertically
    return (this->getX() == destX || this->getY() == destY) && isPathClear(destX, destY, board);
}

char Rook::getSymbol() const
{
    return this->getIsWhite() ? 'R' : 'r';
}

// checking if the rook can move to the new location and there are no pieces blocking its path.
bool Rook::isPathClear(int destX, int destY, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const
{
    int stepX = (destX == this->getX()) ? 0 : (destX > this->getX() ? 1 : -1);
    int stepY = (destY == this->getY()) ? 0 : (destY > this->getY() ? 1 : -1);

    for (int i = this->getX() + stepX, j = this->getY() + stepY; i != destX || j != destY; i += stepX, j += stepY) {
        if (board[i][j] != nullptr) {
            return false;
        }
    }
    return true;
}





