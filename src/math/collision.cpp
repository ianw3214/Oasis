#include "collision.hpp"

namespace Math{

	// takes two generic shapes as input and determines which specific collision function to use
	bool isColliding(Shape& shape1, Shape& shape2) {
		if (shape1.type == RECT && shape2.type == RECT) {
			Rectangle& shape1rect = static_cast<Rectangle&>(shape1);
			Rectangle& shape2rect = static_cast<Rectangle&>(shape2);
			return collisionRectRect(shape1rect, shape2rect);
		}
		if (shape1.type == LINE && shape2.type == LINE) {
			Line& shape1line = static_cast<Line&>(shape1);
			Line& shape2line = static_cast<Line&>(shape2);
			return collisionLineLine(shape1line, shape2line);
		}
		if ((shape1.type == RECT && shape2.type == LINE) || (shape1.type == LINE && shape2.type == RECT)) {
			Rectangle& shapeRect = shape1.type == RECT ? static_cast<Rectangle&>(shape1) : static_cast<Rectangle&>(shape2);
			Line& shapeLine = shape1.type == LINE ? static_cast<Line&>(shape1) : static_cast<Line&>(shape2);
			return collisionLineRect(shapeLine, shapeRect);
		}
		if ((shape1.type == RECT && shape2.type == CIRCLE) || (shape1.type == CIRCLE && shape2.type == RECT)) {
			Rectangle& shapeRect = shape1.type == RECT ? static_cast<Rectangle&>(shape1) : static_cast<Rectangle&>(shape2);
			Circle& shapeCircle = shape1.type == CIRCLE ? static_cast<Circle&>(shape1) : static_cast<Circle&>(shape2);
			return collisionCircleRect(shapeCircle, shapeRect);
		}
		// temporarily return false for any other case.
		return false;
	}

	// calculates if two rect objects are colliding
	bool collisionRectRect(const Rectangle& rect1, const Rectangle& rect2) {
		if (rect1.pos.x < rect2.pos.x + rect2.w && rect1.pos.x + rect1.w > rect2.pos.x &&
			rect1.pos.y < rect2.pos.y + rect2.h && rect1.h + rect1.pos.y > rect2.pos.y) {
			// Collision detected
			return true;
		}
		return false;
	}

	// calculates if two line objects are colliding
	bool collisionLineLine(const Line& line1, const Line& line2) {
		float denominator = static_cast<float>(((line1.pos2.x - line1.pos.x) * (line2.pos2.y - line2.pos.y) - (line1.pos2.y - line1.pos.y) * (line2.pos2.x - line2.pos.x)));
		float numerator1 = static_cast<float>(((line1.pos.y - line2.pos.y) * (line2.pos2.x - line2.pos.x) - (line1.pos.x - line2.pos.x) * (line2.pos2.y - line2.pos.y)));
		float numerator2 = static_cast<float>(((line1.pos.y - line2.pos.y) * (line1.pos2.x - line1.pos.x) - (line1.pos.x - line2.pos.x) * (line1.pos2.y - line1.pos.y)));

		// temporarily return false if the lines are parallel
		if (denominator == 0) return false;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	// calculates if a line object and a rect object is colliding
	bool collisionLineRect(const Line& line, const Rectangle& rect) {
		// separate the rectangles into 4 lines and check for collisions
		Line top(rect.pos.x, rect.pos.y, rect.pos.x + rect.w, rect.pos.y);
		if (collisionLineLine(line, top)) return true;
		Line left(rect.pos.x, rect.pos.y, rect.pos.x, rect.pos.y + rect.h);
		if (collisionLineLine(line, left)) return true;
		Line right(rect.pos.x + rect.w, rect.pos.y, rect.pos.x + rect.w, rect.pos.y + rect.h);
		if (collisionLineLine(line, right)) return true;
		Line bottom(rect.pos.x, rect.pos.y + rect.h, rect.pos.x + rect.w, rect.pos.y + rect.h);
		if (collisionLineLine(line, bottom)) return true;
		// then we check if the line is completely contained in the rect
		if (line.pos.x >= rect.pos.x && line.pos.x <= rect.pos.x + rect.w &&
			line.pos.y >= rect.pos.y && line.pos.y <= rect.pos.y + rect.h) 
		{
			return true;
		}
		return false;
	}

	// calculate if a circle and a rect object is colliding
	bool collisionCircleRect(const Circle& circle, const Rectangle& rect) {
		// find collision by finding shortest point and then comparing distance against radius
		int closestX = clamp(circle.pos.x, rect.pos.x, rect.pos.x + rect.w);
		int closestY = clamp(circle.pos.y, rect.pos.y, rect.pos.y + rect.h);
		int distanceX = circle.pos.x - closestX;
		int distanceY = circle.pos.y - closestY;
		float distanceSquared = static_cast<float>((distanceX * distanceX) + (distanceY * distanceY));
		// if the distance is longer than the radius, the function will return false
		return distanceSquared < (circle.r * circle.r);
	}

}