#include "Exceptions/MoveScoreDontFit.h"

const char* MoveScoreDontFit::what() const noexcept
{
	return "Move has  a lower score then all the moves in the queue.";
}