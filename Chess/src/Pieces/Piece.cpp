#include "Pieces/Piece.h"
//c-tor
Piece::Piece(bool isWhite, int x, int y) : m_isWhite(isWhite), m_Row(x), m_Col(y) {}
//===============================================================================================
bool Piece::getIsWhite() const {
	return this->m_isWhite;
}
//===============================================================================================
int Piece::getRow() const {
	return this->m_Row;
}
//===============================================================================================
int Piece::getCol() const {
	return this->m_Col;
}
//===============================================================================================
void Piece::setPosition(int newX, int newY) {
	this->m_Row = newX;
	this->m_Col = newY;
}