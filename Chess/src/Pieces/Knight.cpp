#include "Pieces/Knight.h"



// c-tor
Knight::Knight(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}

//=============================================================================
bool Knight::isValidMove(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {

    return isValidLShapeMove(destRow, destCol);
}

//=============================================================================
// Knights can jump over pieces, so the path is always clear
bool Knight::isPathClear(int destRow, int destCol,
    const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    return true;
}

//=============================================================================
char Knight::getSymbol() const {
    return this->getIsWhite() ? 'N' : 'n';
}

//=============================================================================
// Helper methods implementation
//=============================================================================

bool Knight::isValidLShapeMove(int destRow, int destCol) const {
    int dRow = std::abs(destRow - this->getRow());
    int dCol = std::abs(destCol - this->getCol());

    // Knight moves in an L-shape: 2 squares in one direction and 1 square perpendicular
    return (dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2);
}

//=============================================================================
// register the knight to the factory
bool Knight::isRegistered = PieceFactory::registerPiece('N', [](bool isWhite, int row, int col) {
    return std::make_shared<Knight>(isWhite, row, col);
    });