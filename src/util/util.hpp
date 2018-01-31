#pragma once

// turn this off when building for release
// #define DEBUG

#ifdef DEBUG
#include <iostream>
#define LOG(x) std::cout << x << std::endl;
#define ERR(x) std::cerr << "ERROR: " << x << std::endl << __FILE__  << ": " << __LINE__ << std::endl;
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define LOG(x)
#define ERR(x)
#define ASSERT(x)
#endif

// limits the @val parameter to the range [min...max]
inline int clamp(int val, int min, int max) {
	return val > max ? max : val < min ? min : val;
}

// limits the @val parameter to the range [min...max]
inline float clamp(float val, float min, float max) {
	return val > max ? max : val < min ? min : val;
}