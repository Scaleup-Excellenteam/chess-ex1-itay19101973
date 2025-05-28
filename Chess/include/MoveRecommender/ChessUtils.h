
#pragma once
namespace ChessUtils {
    // Board dimensions
    const int BOARD_SIZE = 8;

    // Move validation codes
    const int VALID_MOVE = 41;
    const int VALID_MOVE_CHECK = 42;

    // Piece values
    const int PAWN_VALUE = 1;
    const int KNIGHT_VALUE = 3;
    const int BISHOP_VALUE = 3;
    const int ROOK_VALUE = 5;
    const int QUEEN_VALUE = 9;
    const int KING_VALUE = 100;

    // Scoring constants
    const int CAPTURE_MULTIPLIER = 20;
    const int CHECK_BONUS = 50;
    const int CENTER_BONUS_INNER = 15;
    const int CENTER_BONUS_OUTER = 8;
    const int KING_MOVE_PENALTY = -15;
    const int THREAT_PENALTY = -10;
    const int THREAT_MULTIPLIER = 25;
    const int CHECKMATE_SCORE = 1000;
    const int RANDOMNESS_RANGE = 3;

    // Queue settings
    const int MAX_QUEUE_SIZE = 5;

    // Center squares (inner 4 squares)
    const int CENTER_SQUARES_INNER[4][2] = {
        {3, 3}, {3, 4}, {4, 3}, {4, 4}
    };

    // Extended center squares (12 additional squares around center)
    const int CENTER_SQUARES_OUTER[12][2] = {
        {2, 2}, {2, 3}, {2, 4}, {2, 5},
        {3, 2}, {4, 2}, {5, 2}, {5, 3},
        {5, 4}, {5, 5}, {3, 5}, {4, 5}
    };

    // Search optimization
    const int ALPHA_BETA_CUTOFF = 500;

    enum class MoveType {
        NORMAL,
        CAPTURE,
        CHECK,
        CHECKMATE
    };

    enum class PieceType {
        PAWN = 'p',
        KNIGHT = 'n',
        BISHOP = 'b',
        ROOK = 'r',
        QUEEN = 'q',
        KING = 'k'
    };
}
