#include "Pieces/Queen.h"


// c- tor
Queen::Queen(bool isWhite, int row, int col) : Piece(isWhite, row, col) {}

//==========================================================================
// validating if the move is leagal
bool Queen::isValidMove(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    // Queen can move like a Rook or a Bishop 
    int dRow = std::abs(destRow - this->getRow());
    int dCol = std::abs(destCol - this->getCol());

    bool straightMove = (this->getRow() == destRow || this->getCol() == destCol);
    bool diagonalMove = (dRow == dCol) && (dRow > 0);

    return (straightMove || diagonalMove) && isPathClear(destRow, destCol, board);
}

//==========================================================================
char Queen::getSymbol() const {
    return this->getIsWhite() ? 'Q' : 'q';
}

//==========================================================================
//checks if the path of the queen is empty 
bool Queen::isPathClear(int destRow, int destCol, const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const {
    // Ensure destination is within bounds
    if (destRow < 0 || destRow >= 8 || destCol < 0 || destCol >= 8) {
        return false;
    }

    // Direction of movement
    int stepRow = 0;
    int stepCol = 0;


    //down
    if (destRow > this->getRow()) {
        stepRow = 1;
    }
    //up
    else if (destRow < this->getRow()) {
        stepRow = -1;
    }
    //right
    if (destCol > this->getCol()) {
        stepCol = 1;
    }
    //left
    else if (destCol < this->getCol()) {
        stepCol = -1;
    }

    int i = this->getRow() + stepRow;
    int j = this->getCol() + stepCol;

    while (i != destRow || j != destCol) {
        if (board[i][j] != nullptr) {
            return false; // blocked by another piece
        }
        i += stepRow;
        j += stepCol;
    }

    return true; // path clear
}
//==========================================================================
// register the queen to the factory
bool Queen::isRegistered = PieceFactory::registerPiece('Q', [](bool isWhite, int x, int y) {
    return std::make_shared<Queen>(isWhite, x, y);
});
