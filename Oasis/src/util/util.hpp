#pragma once

#include "trap.hpp"
#include "pointer.hpp"

#include "colour.hpp"

// Common functions
template<typename T>
T lerp(const T& start, const T& end, float percentage) {
	return start + static_cast<T>(static_cast<float>(end - start) * percentage);
}