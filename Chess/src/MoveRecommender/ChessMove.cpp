
#include "MoveRecommender/ChessMove.h"
//=====================================================================
ChessMove::ChessMove(const std::string& source, const std::string& dest, bool forWhite, int moveScore)
    : m_sourcePos(source), m_destPos(dest), m_score(moveScore), m_isWhite(forWhite) {
}
//=====================================================================
ChessMove::ChessMove(const ChessMove& other)
    : m_sourcePos(other.m_sourcePos), 
      m_destPos(other.m_destPos),
      m_score(other.m_score),
      m_isWhite(other.m_isWhite) {
}
//=====================================================================
ChessMove& ChessMove::operator=(const ChessMove& other) {
    if (this != &other) {
        m_sourcePos = other.m_sourcePos;
        m_destPos = other.m_destPos;
        m_score = other.m_score;
        m_isWhite = other.m_isWhite;
    }
    return *this;
}
//=====================================================================
bool ChessMove::operator==(const ChessMove& other) const {
    return m_sourcePos == other.m_sourcePos && m_destPos == other.m_destPos;
}
//=====================================================================
std::string ChessMove::getSourcePos() const {
    return m_sourcePos;
}
//=====================================================================
std::string ChessMove::getDestPos() const {
    return m_destPos;
}
//=====================================================================
int ChessMove::getScore() const {
    return m_score;
}
//=====================================================================
bool ChessMove::getIsWhite() const {
    return m_isWhite;
}
//=====================================================================
void ChessMove::setScore(int score) {
    m_score = score;
}
//=====================================================================
std::string ChessMove::toString() const {
    return m_sourcePos + m_destPos + " (Score: " + std::to_string(m_score) + ")";
}

std::ostream& operator<<(std::ostream& os, const ChessMove& move) {
    os << move.toString();
    return os;
}
