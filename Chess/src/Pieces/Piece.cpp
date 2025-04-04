#include "Pieces/Piece.h"

Piece::Piece(bool isWhite, int x, int y) : m_isWhite(isWhite), m_x(x), m_y(y) {}

bool Piece::getIsWhite() const {
	return this->m_isWhite;
}

int Piece::getX() const {
	return this->m_x;
}

int Piece::getY() const {
	return this->m_y;
}

void Piece::setPosition(int newX, int newY) {
	this->m_x = newX;
	this->m_y = newY;
}