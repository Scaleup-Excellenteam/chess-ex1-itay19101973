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
                whiteKingX = destRow;
                whiteKingY = destCol;
            }
            else {
                blackKingX = destRow;
                blackKingY = destCol;
            }
        }

        // Switch turn
        isWhiteTurn = !isWhiteTurn;
    }

    // Check if king of specified color is in check
    bool isKingInCheck(bool isWhiteKing) {
        int kingX = isWhiteKing ? whiteKingX : blackKingX;
        int kingY = isWhiteKing ? whiteKingY : blackKingY;

        // Check if any opponent piece can attack the king
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                std::shared_ptr<Piece> piece = board[x][y];
                if (piece && piece->getIsWhite() != isWhiteKing &&
                    piece->isValidMove(kingX, kingY, board) &&
                    piece->isPathClear(kingX, kingY, board)) {
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
                oldKingX = whiteKingX;
                oldKingY = whiteKingY;
                whiteKingX = destX;
                whiteKingY = destY;
            }
            else {
                oldKingX = blackKingX;
                oldKingY = blackKingY;
                blackKingX = destX;
                blackKingY = destY;
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
                whiteKingX = oldKingX;
                whiteKingY = oldKingY;
            }
            else {
                blackKingX = oldKingX;
                blackKingY = oldKingY;
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
    int whiteKingX, whiteKingY;
    int blackKingX, blackKingY;

    int validateBasicRules(int srcX, int srcY, int  destX, int destY) const;
    int validatePieceMovement(int srcX, int srcY, int destX, int destY)const;
    bool isKingMoving(std::shared_ptr<Piece> piece)const;
    void updateKingPos(std::shared_ptr<Piece> piece, int& oldKingX, int& oldKingY, const int& destX, const int& destY);
    void restoreBoardPos(std::shared_ptr<Piece> piece, std::shared_ptr<Piece> capturedPiece,
        int srcX, int srcY, int destX, int destY, bool isKingMoving);
};



