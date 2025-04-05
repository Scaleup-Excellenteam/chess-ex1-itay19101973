#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Pieces/Piece.h"
#include "PieceFactory/PieceFactory.h"
#include "Pieces/King.h"


class Board {

public:
    Board(const std::string& initialBoard);

    std::pair<int, int> notationToCoordinates(std::string notation); 
        

    // Check if a move is valid and return the appropriate code
    int validateMove(const std::string& source, const std::string& dest);

    // Make a move on the board
    void makeMove(const std::string& source, const std::string& dest) {
        auto [srcRow, srcCol] = notationToCoordinates(source);
        auto [destRow, destCol] = notationToCoordinates(dest);

        std::shared_ptr<Piece> piece = board[srcRow][srcCol];
        board[destRow][destCol] = piece;
        board[srcRow][srcCol] = nullptr;

        // Update piece position
        piece->setPosition(destRow, destCol);

        // Update king position if the king is moving
        if (std::dynamic_pointer_cast<King>(piece)) {
            if (piece->getIsWhite()) {
                whiteKingRow = destRow;
                whiteKingCol = destCol;
            }
            else {
                blackKingRow = destRow;
                blackKingCol = destCol;
            }
        }

        // Switch turn
        isWhiteTurn = !isWhiteTurn;
    }

    // Check if king of specified color is in check
    bool isKingInCheck(bool isWhiteKing) {
        int kingRow = isWhiteKing ? whiteKingRow : blackKingRow;
        int kingCol = isWhiteKing ? whiteKingCol : blackKingCol;

        // Check if any opponent piece can attack the king
        for (int col = 0; col < 8; col++) {
            for (int row = 0; row < 8; row++) {
                std::shared_ptr<Piece> piece = board[row][col];
                if (piece && piece->getIsWhite() != isWhiteKing &&
                    piece->isValidMove(kingRow, kingCol, board) &&
                    piece->isPathClear(kingRow, kingCol, board)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Check if a move would cause the player's king to be in check
    bool wouldCauseCheck(int srcX, int srcY, int destX, int destY) {
        std::shared_ptr<Piece> piece = board[srcX][srcY];
        std::shared_ptr<Piece> capturedPiece = board[destX][destY];

        // Make the move temporarily
        board[destX][destY] = piece;
        board[srcX][srcY] = nullptr;

        // Update piece position
        int oldX = piece->getRow();
        int oldY = piece->getCol();
        piece->setPosition(destX, destY);

        // Update king position if the king is moving
        bool isKingMoving = std::dynamic_pointer_cast<King>(piece) != nullptr;
        int oldKingX = -1, oldKingY = -1;
        if (isKingMoving) {
            if (piece->getIsWhite()) {
                oldKingX = whiteKingRow;
                oldKingY = whiteKingCol;
                whiteKingRow = destX;
                whiteKingCol = destY;
            }
            else {
                oldKingX = blackKingRow;
                oldKingY = blackKingCol;
                blackKingRow = destX;
                blackKingCol = destY;
            }
        }

        // Check if king is in check after the move
        bool kingInCheck = isKingInCheck(isWhiteTurn);

        // Restore the board
        board[srcX][srcY] = piece;
        board[destX][destY] = capturedPiece;
        piece->setPosition(oldX, oldY);

        // Restore king position if it was moved
        if (isKingMoving) {
            if (piece->getIsWhite()) {
                whiteKingRow = oldKingX;
                whiteKingCol = oldKingY;
            }
            else {
                blackKingRow = oldKingX;
                blackKingCol = oldKingY;
            }
        }

        return kingInCheck;
    }

    // Get current turn
    bool getIsWhiteTurn() const {
        return isWhiteTurn;
    }

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> board;
    bool isWhiteTurn;
    int whiteKingRow, whiteKingCol;
    int blackKingRow, blackKingCol;

    int validateBasicRules(int srcX, int srcY, int  destX, int destY) const;
    int validatePieceMovement(int srcX, int srcY, int destX, int destY)const;
    bool isKingMoving(std::shared_ptr<Piece> piece)const;
    void updateKingPos(std::shared_ptr<Piece> piece, int& oldKingX, int& oldKingY, const int& destX, const int& destY);
    void restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
        int srcX, int srcY, int destX, int destY, bool isKingMoving);
};



