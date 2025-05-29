#pragma once
#include <exception>
#include <string>

// Custom exception for empty queue
class MoveScoreDontFit : public std::exception {
public:
    
    const char* what() const noexcept override;
};