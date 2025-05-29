#include "Exceptions/EmptyQueueException.h"

const char* EmptyQueueException::what() const noexcept
{
	return "PriorityQueue is empty. Cannot poll an element.";
}
