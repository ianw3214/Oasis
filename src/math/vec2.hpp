#pragma once


struct Vec2 {
	int x;
	int y;
	Vec2() : x(0), y(0) {}
	Vec2(int _x, int _y) : x(_x), y(_y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}
	Vec2 operator=(const Vec2& v) {
		x = v.x;
		y = v.y;
		return Vec2(v.x, v.y);
	}
	bool operator==(const Vec2& other) {
		return x == other.x && y == other.y;
	}
};