#include "Pieces/Rook.h"

Rook::Rook(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}
//=================================================================================
// checks if its a valid rook move
bool Rook::isValidMove(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    return (this->getRow() == destRow || this->getCol() == destCol) && isPathClear(destRow, destCol, board);
}

char Rook::getSymbol() const {
    return this->getIsWhite() ? 'R' : 'r';
}
//=================================================================================
// Checks if there are no pieces blocking the Rook's path
bool Rook::isPathClear(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    int stepRow = (destRow == this->getRow()) ? 0 : (destRow > this->getRow() ? 1 : -1);
    int stepCol = (destCol == this->getCol()) ? 0 : (destCol > this->getCol() ? 1 : -1);

    int i = this->getRow() + stepRow;
    int j = this->getCol() + stepCol;

    while (i != destRow || j != destCol) {
        if (board[i][j] != nullptr) {
            return false; // blocked by another piece
        }
        i += stepRow;
        j += stepCol;
    }

    return true;// path is clear
}

//=================================================================================
// register to the factory
bool Rook::isRegistered = PieceFactory::registerPiece('R', [](bool isWhite, int x, int y) {
    return std::make_shared<Rook>(isWhite, x, y);
    });