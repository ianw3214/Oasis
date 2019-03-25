#pragma once

#include "vec2.hpp"
#include "../util/util.hpp"
namespace Math {

	enum SHAPE_TYPE {
		RECT = 0,
		LINE = 1,
		CIRCLE = 2
	};

	struct Shape {
		Vec2 pos;
		SHAPE_TYPE type;
		Shape() {}
		Shape(SHAPE_TYPE t) : type(t) {}
		Shape(int _x, int _y) : pos(_x, _y) {}
		Shape(int _x, int _y, SHAPE_TYPE t) : type(t), pos(_x, _y) {}
	};

	struct Rectangle : Shape {
		int w, h;
		Rectangle() : Shape(RECT) {}
		Rectangle(int _x, int _y) : Shape(_x, _y, RECT) {}
		Rectangle(int _x, int _y, int _w, int _h) : Shape(_x, _y, RECT), w(_w), h(_h) {}
		// copy constructor
		Rectangle(const Rectangle &obj) : Shape(obj.pos.x, obj.pos.y, RECT), w(obj.w), h(obj.h) {}
	};

	struct Line : Shape {
		Vec2 pos2;
		Line() : Shape(LINE) {}
		Line(int _x1, int _y1) : Shape(_x1, _y1, LINE) {}
		Line(int _x1, int _y1, int _x2, int _y2) : Shape(_x1, _y1, LINE) {
			pos2.x = _x2;
			pos2.y = _y2;
		}
		// copy constructor
		Line(const Line &obj) : Line(obj.pos.x, obj.pos.y, obj.pos2.x, obj.pos2.y) {}
	};

	struct Circle : Shape {
		int r;
		Circle() : Shape(CIRCLE) {}
		Circle(int x, int y, int rIn) : Shape(x, y, CIRCLE), r(rIn) {}
		// copy constructor
		Circle(const Circle &obj) : Circle(obj.pos.x, obj.pos.y, obj.r) {}
	};

	// pass shape parameters by reference to avoid slicing	

	// functions to get collisions
	bool isColliding(const Shape&, const Shape&);
	bool isColliding(const Vec2&, const Shape&);

	// helper functions for different collisions of different shapes
	bool collisionRectRect(const Rectangle&, const Rectangle&);
	bool collisionLineLine(const Line&, const Line&);
	bool collisionLineRect(const Line&, const Rectangle&);
	bool collisionCircleRect(const Circle&, const Rectangle&);
	bool collisionVecRect(const Vec2&, const Rectangle&);
	bool collisionVecLine(const Vec2&, const Line&);
	bool collisionVecCircle(const Vec2&, const Circle&);

}