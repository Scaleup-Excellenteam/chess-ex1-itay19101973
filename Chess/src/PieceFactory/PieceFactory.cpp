#include "PieceFactory/PieceFactory.h"

std::map<char, PieceFactory::Creator>& PieceFactory::getCreators() {
    static std::map<char, Creator> creators; // static pieces map factory
    return creators;
}


// adding a piece to the factory
bool PieceFactory::registerPiece(char symbol, Creator creator) {
    char upperSymbol = std::toupper(symbol);
    auto& creators = getCreators();
    if (creators.find(upperSymbol) != creators.end()) {
        return false;
    }
    creators[upperSymbol] = creator;
    return true;
}


// create the  piece
std::shared_ptr<Piece> PieceFactory::createPiece(char symbol, int x, int y) {
    bool isWhite = std::isupper(symbol);
    char upperSymbol = std::toupper(symbol);

    auto& creators = getCreators();
    auto it = creators.find(upperSymbol);
    if (it != creators.end()) {
        return it->second(isWhite, x, y);
    }
    return nullptr;
}


