
#include "Pieces/Pawn.h"

// c-tor
Pawn::Pawn(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}

//=============================================================================
bool Pawn::isValidMove(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    // Check each possible pawn move type
    if (isValidForwardMove(destRow, destCol, board) ||
        isValidDoubleMove(destRow, destCol, board) || 
        isValidDiagonalCapture(destRow, destCol, board)) {

        return true;
    }



    return false;
}

//=============================================================================
bool Pawn::isPathClear(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    // For diagonal captures, no path to check
    int dCol = destCol - this->getCol();
    if (std::abs(dCol) == 1) {
        return true;
    }

    // For forward movement of 2 squares, check the middle square
    int dRow = destRow - this->getRow();
    if (std::abs(dRow) == 2) {
        int midRow = this->getRow() + getForwardDirection();
        return board[midRow][destCol] == nullptr;
    }

    return true;
}

//=============================================================================
char Pawn::getSymbol() const {
    return this->getIsWhite() ? 'P' : 'p';
}

//=============================================================================
// Helper methods implementation
//=============================================================================

int Pawn::getForwardDirection() const {
    // White moves down the board (increasing row), black moves up (decreasing row)
    return this->getIsWhite() ? 1 : -1;
}

//=============================================================================
bool Pawn::isAtStartingPosition() const {
    // White pawns start at row 1, black pawns at row 6
    return (this->getIsWhite() && this->getRow() == 1) ||
        (!this->getIsWhite() && this->getRow() == 6);
}

//=============================================================================
bool Pawn::isValidForwardMove(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    int dRow = destRow - this->getRow();
    int dCol = destCol - this->getCol();

    // Check if moving one square forward
    if (dCol == 0 && dRow == getForwardDirection()) {
        // Destination must be empty
        return board[destRow][destCol] == nullptr;
    }

    return false;
}

//=============================================================================
bool Pawn::isValidDoubleMove(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    int dRow = destRow - this->getRow();
    int dCol = destCol - this->getCol();

    // Check if making the initial two-square move
    if (dCol == 0 && dRow == 2 * getForwardDirection() && isAtStartingPosition()) {
        // Both the destination and the square in between must be empty
        int midRow = this->getRow() + getForwardDirection();
        return board[midRow][destCol] == nullptr && board[destRow][destCol] == nullptr;
    }

    return false;
}

//=============================================================================
bool Pawn::isValidDiagonalCapture(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    int dRow = destRow - this->getRow();
    int dCol = destCol - this->getCol();

    // Check if moving diagonally forward
    if (std::abs(dCol) == 1 && dRow == getForwardDirection()) {
        // Can only move diagonally if capturing an opponent's piece
        return board[destRow][destCol] != nullptr &&
            board[destRow][destCol]->getIsWhite() != this->getIsWhite();
    }

    return false;
}

//=============================================================================
// register the pawn to the factory
bool Pawn::isRegistered = PieceFactory::registerPiece('P', [](bool isWhite, int row, int col) {
    return std::make_shared<Pawn>(isWhite, row, col);
    });