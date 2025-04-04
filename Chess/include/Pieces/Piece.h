#pragma once

#include <string>
#include <vector>
#include <memory>

class Piece {


public:
    Piece(bool isWhite, int x, int y);
    virtual ~Piece() = default;

    
    virtual bool isValidMove(int destX, int destY, 
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0; 

    


    // Getters
    bool getIsWhite() const;
    int getX() const;
    int getY() const;

    // Setters
    void setPosition(int newX, int newY);

    virtual char getSymbol() const = 0;

private:
    virtual bool isPathClear(int destX, int destY,
        const std::vector<std::vector<std::shared_ptr<Piece>>>& board) const = 0;

    bool m_isWhite;
    int m_x, m_y;
};