#pragma once


#include "Pieces/Piece.h"
#include <functional>
#include <map>
#include <memory>


/**
 * @class PieceFactory
 *
 * This class implements the Factory design pattern to allow creation of different
 * chess pieces based on a character symbol. It supports runtime registration of
 * piece types and uses function pointers or lambdas to instantiate them.
 */

class PieceFactory {
public:
    using Creator = std::function<std::shared_ptr<Piece>(bool, int, int)>;

    static bool registerPiece(char symbol, Creator creator);
    static std::shared_ptr<Piece> createPiece(char symbol, int x, int y);

private:
    static std::map<char, Creator>& getCreators();
};