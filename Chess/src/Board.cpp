#include "Board.h"


//builds the tools board matrics
Board::Board(const std::string& initialBoard)
{
    // Initialize an 8x8 board
    board.resize(8, std::vector<std::shared_ptr<Piece>>(8, nullptr));
    isWhiteTurn = true;

    // Initialize the board using the factory
    int index = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            char symbol = initialBoard[index++]; // moving on the string
            if (symbol != '#') {
                board[row][col] = PieceFactory::createPiece(symbol, row, col);

                // Track kings' positions
                if (symbol == 'K') {
                    whiteKingRow = row;
                    whiteKingCol = col;
                }
                else if (symbol == 'k') {
                    blackKingRow = row;
                    blackKingCol = col;
                }
            }
        }
    }
}


// Convert chess notation to board coordinates (e.g., "A1" -> (0,7))
std::pair<int, int> Board::notationToCoordinates(std::string notation)
{
    char file = toupper(notation[0]);  // A-H
    char rank = notation[1];           // 1-8

    int row = file - 'A';      // 'A' → 0, ..., 'H' → 7
    int col = rank - '1';      // '1' → 0, ..., '8' → 7

    return { row, col };
}

int Board::validateMove(const std::string& source, const std::string& dest) {
    // Convert chess notation to board coordinates
    auto [srcRow, srcCol] = notationToCoordinates(source);
    auto [destRow, destCol] = notationToCoordinates(dest);

    int valid = 0;

    valid = this->validateBasicRules(srcRow, srcCol, destRow, destCol);
    if (valid != 0) {
        return valid;
    }

    valid = this->validatePieceMovement(srcRow, srcCol, destRow, destCol);
    if (valid != 0) {
        return valid;
    }

    std::shared_ptr<Piece> piece = board[srcRow][srcCol];
    // Execute the move temporarily
    std::shared_ptr<Piece> capturedPiece = this->board[destRow][destCol];
    this->board[destRow][destCol] = piece;
    this->board[srcRow][srcCol] = nullptr;

    // Update piece position
    int oldRow = piece->getRow();
    int oldCol = piece->getCol();
    piece->setPosition(destRow, destCol);

    // Update king position if the king is moving
    int oldKingRow = -1, oldKingCol = -1;
    bool isKingMoving = this->isKingMoving(piece);
    if (isKingMoving) {
        updateKingPos(piece, oldKingRow, oldKingCol, destRow, destCol);
    }

    
    bool selfCheck = this->isKingInCheck(isWhiteTurn);
    if (selfCheck) {
        restoreBoardPos(piece, capturedPiece, srcRow, srcCol, destRow, destCol, isKingMoving);
        return 31;
    }
    bool causesCheck = isKingInCheck(!isWhiteTurn);

    // Restore the board and piece position
    restoreBoardPos(piece, capturedPiece, srcRow, srcCol, destRow, destCol, isKingMoving);


    
    
    
    // Return appropriate code
    return causesCheck ? 41 : 42;
}



// this function validates that the basic rule games are applied
int Board::validateBasicRules(int srcRow, int srcCol, int destRow, int destCol) const
{
    std::shared_ptr<Piece> piece = board[srcRow][srcCol];

    // Check that there is a piece at the source
    if (piece == nullptr) {
        return 11; // No piece at source
    }

    // Check if the piece belongs to the current player
    if (piece->getIsWhite() != isWhiteTurn) {
        return 12; // Piece belongs to opponent
    }

    // Check if destination has a piece of the same color
    if (board[destRow][destCol] != nullptr && board[destRow][destCol]->getIsWhite() == isWhiteTurn) {
        return 13; // Destination has a piece of the same color
    }

    return 0; // All basic rules passed
}




int Board::validatePieceMovement(int srcRow, int srcCol, int destRow, int destCol) const {
    std::shared_ptr<Piece> piece = board[srcRow][srcCol];

    // Check if the move is valid for this piece
    if (!piece->isValidMove(destRow, destCol, board) || !piece->isPathClear(destRow, destCol, board)) {
        return 21; // Invalid movement for this piece
    }

    return 0; // Movement is valid
}

bool Board::isKingMoving(std::shared_ptr<Piece> piece) const{

    char symbol = piece->getSymbol();
    if (symbol == 'k' || symbol == 'K') {
         return true;
    }
    return false;

}

void Board::updateKingPos(std::shared_ptr<Piece> piece, int& oldKingRow, int& oldKingCol ,const int& destRow , const int& destCol) {

    if (piece->getIsWhite()) {
        oldKingRow = whiteKingRow;
        oldKingCol = whiteKingCol;
        whiteKingRow = destRow;
        whiteKingCol = destCol;
    }
    else {
        oldKingRow = blackKingRow;
        oldKingCol = blackKingCol;
        blackKingRow = destRow;
        blackKingCol = destCol;
    }

}

void Board::restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
    int srcRow, int srcCol, int destRow, int destCol, bool isKingMoving) {
    this->board[srcRow][srcCol] = piece;
    board[destRow][destCol] = capturedPiece;
    piece->setPosition(srcRow, srcCol);

    // Restore king position if it was moved
    if (isKingMoving) {
        if (piece->getIsWhite()) {
            this->whiteKingRow = srcRow;
            this->whiteKingCol = srcCol;
        }
        else {
            this->blackKingRow = srcRow;
            this->blackKingCol = srcCol;
        }
    }
}


