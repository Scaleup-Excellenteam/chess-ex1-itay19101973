#pragma once


#include "Pieces/Piece.h"
#include <functional>
#include <map>
#include <memory>

class PieceFactory {
public:
    using Creator = std::function<std::shared_ptr<Piece>(bool, int, int)>;

    static bool registerPiece(char symbol, Creator creator);
    static std::shared_ptr<Piece> createPiece(char symbol, int x, int y);

private:
    static std::map<char, Creator>& getCreators();
};