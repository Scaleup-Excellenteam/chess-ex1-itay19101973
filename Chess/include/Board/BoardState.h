#pragma once
#include<vector>
#include "Pieces/Piece.h"


// Structure to save board state
struct BoardState {
    // The complete board state (8x8 grid of pieces)
    std::vector<std::vector<std::shared_ptr<Piece>>> boardGrid;

    // Current turn
    bool isWhiteTurn;

    // King positions
    int whiteKingRow;
    int whiteKingCol;
    int blackKingRow;
    int blackKingCol;
};