#pragma once

#include <string>
#include <map>

// turn this off when building for release
#define DEBUG

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

// linked list node to represent configuration variables
struct CVAR {
	CVAR() {}
	~CVAR() {}
	std::string name;
	union {
		int i_val;
		float f_val;
		std::string s_val;
	};
	CVAR * next;
};

// profiling tools
struct PROFILE_DATA {
	unsigned int start;
	unsigned int total;
	unsigned int count;
};