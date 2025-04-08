#include "Pieces/Bishop.h"


Bishop::Bishop(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}

// checks if the move is valid
bool Bishop::isValidMove(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    // Bishop can only move diagonally
    int dRow = std::abs(destRow - this->getRow());
    int dCol = std::abs(destCol - this->getCol());

    // For diagonal movement, the absolute difference in row and column must be equal
    return (dRow == dCol) && (dRow > 0) && isPathClear(destRow, destCol, board);
}



//returns the symbol of the bishop
char Bishop::getSymbol() const {
    return this->getIsWhite() ? 'B' : 'b';
}


// checks that there are no pieces blocking the bishop from moving
bool Bishop::isPathClear(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    // Ensure destination is within bounds
    if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
        return false;
    }

    // Direction of movement
    int stepRow = (destRow > this->getRow()) ? 1 : -1;
    int stepCol = (destCol > this->getCol()) ? 1 : -1;

    int i = this->getRow() + stepRow;
    int j = this->getCol() + stepCol;

    while (i != destRow && j != destCol) {
        if (board[i][j] != nullptr) {
            return false; // Blocked by another piece
        }
        i += stepRow;
        j += stepCol;
    }

    return true; // move is ok
}

// register bishop to the factory
bool Bishop::isRegistered = PieceFactory::registerPiece('B', [](bool isWhite, int x, int y) {
    return std::make_shared<Bishop>(isWhite, x, y);
    });