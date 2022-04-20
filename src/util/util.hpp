#pragma once

#include "trap.hpp"
#include "pointer.hpp"

#include "colour.hpp"

struct Point {
	int x = 0;
	int y = 0;	
};

struct AABB {
	Point min;
	Point max;
};

// Common functions
template<typename T>
T lerp(const T& start, const T& end, float percentage) {
	return start + static_cast<T>(static_cast<float>(end - start) * percentage);
}

// 'Collision' functions
inline bool IsPointInAABB(int x, int y, const AABB& aabb) {
	const bool hCheck = x >= aabb.min.x && x <= aabb.max.x;
	const bool vCheck = y >= aabb.min.y && y <= aabb.max.y;
	return hCheck && vCheck;
}

inline bool IsPointInAABB(const Point& point, const AABB& aabb) {
	return IsPointInAABB(point.x, point.y, aabb);
}