#include "Pieces/Rook.h"

Rook::Rook(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}

bool Rook::isValidMove(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    // Rook can only move in straight lines (same row or same column)
    return (this->getRow() == destRow || this->getCol() == destCol) && isPathClear(destRow, destCol, board);
}

char Rook::getSymbol() const {
    return this->getIsWhite() ? 'R' : 'r';
}

// Checks if there are no pieces blocking the Rook's path
bool Rook::isPathClear(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    // Ensure destination is within bounds
    if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
        return false;
    }

    int stepRow = (destRow == this->getRow()) ? 0 : (destRow > this->getRow() ? 1 : -1);
    int stepCol = (destCol == this->getCol()) ? 0 : (destCol > this->getCol() ? 1 : -1);

    int i = this->getRow() + stepRow;
    int j = this->getCol() + stepCol;

    while (i != destRow || j != destCol) {
        if (board[i][j] != nullptr) {
            return false; // Blocked by another piece
        }
        i += stepRow;
        j += stepCol;
    }

    return true;
}
