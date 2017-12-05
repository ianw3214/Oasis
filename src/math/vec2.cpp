#include "vec2.hpp"	

// basic operators for vectors
Vec2 operator+(const Vec2& a, const Vec2& b) {
	return Vec2(a.x + b.x, a.y + b.y);
}

Vec2 operator*(const Vec2& a, const Vec2& b) {
	return Vec2(a.x * b.x, a.y * b.y);
}
