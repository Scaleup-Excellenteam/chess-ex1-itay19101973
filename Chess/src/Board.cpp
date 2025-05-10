#include "Board.h"


//builds the tools board matrics
Board::Board(const std::string& initialBoard) 
{
    // Initialize an 8x8 board
    m_board.resize(8, std::vector<std::shared_ptr<Piece>>(8, nullptr));
    m_isWhiteTurn = true;

    // Initialize the board using the factory
    int index = 0;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            char symbol = initialBoard[index++]; // moving on the string
            if (symbol != '#') {
                m_board[row][col] = PieceFactory::createPiece(symbol, row, col);

                // Track kings' positions
                if (symbol == 'K') {
                    m_whiteKingRow = row;
                    m_whiteKingCol = col;
                }
                else if (symbol == 'k') {
                    m_blackKingRow = row;
                    m_blackKingCol = col;
                }
            }
        }
    }
}

//=================================================================================================
// Convert chess notation to board coordinates
std::pair<int, int> Board::notationToCoordinates(std::string notation)
{
    char file = toupper(notation[0]);  // A-H
    char rank = notation[1];           // 1-8

    int row = file - 'A';     
    int col = rank - '1';      

    return { row, col };
}
//=================================================================================================
//this function validates if the move is ok 
//for example it checks if the piece that is moving is of the 
// right player
// returns the right code for the engine
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

    std::shared_ptr<Piece> piece = m_board[srcRow][srcCol];
    // Execute the move temporarily
    std::shared_ptr<Piece> capturedPiece = this->m_board[destRow][destCol];
    this->m_board[destRow][destCol] = piece;
    this->m_board[srcRow][srcCol] = nullptr;

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

    
    bool selfCheck = this->isKingInCheck(m_isWhiteTurn);
    if (selfCheck) {
        restoreBoardPos(piece, capturedPiece, srcRow, srcCol, destRow, destCol, isKingMoving);
        return 31;
    }
    bool causesCheck = isKingInCheck(!m_isWhiteTurn);

    // Restore the board and piece position
    restoreBoardPos(piece, capturedPiece, srcRow, srcCol, destRow, destCol, isKingMoving);    
    // Return appropriate code
    return causesCheck ? 41 : 42;
}
//=================================================================================================
//function executes the move 
void Board::makeMove(const std::string& source, const std::string& dest)
{
    auto [srcRow, srcCol] = notationToCoordinates(source);
    auto [destRow, destCol] = notationToCoordinates(dest);

    std::shared_ptr<Piece> piece = m_board[srcRow][srcCol];
    m_board[destRow][destCol] = piece;
    m_board[srcRow][srcCol] = nullptr;

    // Update piece position
    piece->setPosition(destRow, destCol);

    // Update king position if the king is moving
    if (std::dynamic_pointer_cast<King>(piece)) {
        if (piece->getIsWhite()) {
            m_whiteKingRow = destRow;
            m_whiteKingCol = destCol;
        }
        else {
            m_blackKingRow = destRow;
            m_blackKingCol = destCol;
        }
    }

    // Switch turn
    m_isWhiteTurn = !m_isWhiteTurn;
}
//===============================================================
// returns piece at given coordinates
std::shared_ptr<Piece> Board::getPieceAt(int row, int col) const
{
    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        return m_board[row][col];
    }
    return nullptr;
}

//===============
int Board::getWhiteKingRow() const
{
    return this->m_whiteKingRow;
}
//==================
int Board::getWhiteKingCol() const
{
    return this->m_whiteKingCol;
}
//=================
int Board::getBlackKingRow() const
{
    return this->m_blackKingRow;
}
int Board::getBlackKingCol() const
{
    return this->m_blackKingCol;
}
//=================================================================================================
// function check if the given king is in check
bool Board::isKingInCheck(bool isWhiteKing)
{
    int kingRow = isWhiteKing ? m_whiteKingRow : m_blackKingRow;
    int kingCol = isWhiteKing ? m_whiteKingCol : m_blackKingCol;

    // Check if any opponent piece can attack the king
    for (int col = 0; col < 8; col++) {
        for (int row = 0; row < 8; row++) {
            std::shared_ptr<Piece> piece = m_board[row][col];
            if (piece && piece->getIsWhite() != isWhiteKing &&
                piece->isValidMove(kingRow, kingCol, m_board) &&
                piece->isPathClear(kingRow, kingCol, m_board)) {
                return true;
            }
        }
    }
    return false;
}
//=================================================================================================
// function returns true if its whight turn
bool Board::getIsWhiteTurn() const
{
    return m_isWhiteTurn;
}


//=================================================================================================
// this function validates that the basic rule games are applied
int Board::validateBasicRules(int srcRow, int srcCol, int destRow, int destCol) const
{
    std::shared_ptr<Piece> piece = m_board[srcRow][srcCol];

    // Check that there is a piece at the source
    if (piece == nullptr) {
        return 11; // No piece at source
    }

    // Check if the piece belongs to the current player
    if (piece->getIsWhite() != m_isWhiteTurn) {
        return 12; // Piece belongs to opponent
    }

    // Check if destination has a piece of the same color
    if (m_board[destRow][destCol] != nullptr && m_board[destRow][destCol]->getIsWhite() == m_isWhiteTurn) {
        return 13; // Destination has a piece of the same color
    }

    return 0; // All basic rules passed
}

//=================================================================================================
// this function checks  if the piece can move correctly, for example
// bishop can only move diagonly
int Board::validatePieceMovement(int srcRow, int srcCol, int destRow, int destCol) const {
    std::shared_ptr<Piece> piece = m_board[srcRow][srcCol];

    // Check if the move is valid for this piece
    if (!piece->isValidMove(destRow, destCol, m_board) || !piece->isPathClear(destRow, destCol, m_board)) {
        return 21; // Invalid movement for this piece
    }

    return 0; // Movement is valid
}
//=================================================================================================
//function checks if the piece that moves is the king
bool Board::isKingMoving(std::shared_ptr<Piece> piece) const{

    char symbol = piece->getSymbol();
    if (symbol == 'k' || symbol == 'K') {
         return true;
    }
    return false;

}
//=================================================================================================
// this function keep tracks of the king movement
void Board::updateKingPos(std::shared_ptr<Piece> piece, int& oldKingRow, int& oldKingCol ,const int& destRow , const int& destCol) {

    if (piece->getIsWhite()) {
        oldKingRow = m_whiteKingRow;
        oldKingCol = m_whiteKingCol;
        m_whiteKingRow = destRow;
        m_whiteKingCol = destCol;
    }
    else {
        oldKingRow = m_blackKingRow;
        oldKingCol = m_blackKingCol;
        m_blackKingRow = destRow;
        m_blackKingCol = destCol;
    }

}
//=================================================================================================
// after temporarly executing the move to check validation restoring the board position back.
void Board::restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
    int srcRow, int srcCol, int destRow, int destCol, bool isKingMoving) {
    this->m_board[srcRow][srcCol] = piece;
    m_board[destRow][destCol] = capturedPiece;
    piece->setPosition(srcRow, srcCol);

    // Restore king position if it was moved
    if (isKingMoving) {
        if (piece->getIsWhite()) {
            this->m_whiteKingRow = srcRow;
            this->m_whiteKingCol = srcCol;
        }
        else {
            this->m_blackKingRow = srcRow;
            this->m_blackKingCol = srcCol;
        }
    }
}


