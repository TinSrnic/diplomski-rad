#include "Vec2i.h"
#include "Vec3i.h"

Vec2i::Vec2i(const int x, const int y) {
	this->x = x;
	this->y = y;
}
Vec2i::Vec2i(const Vec2i& other) : Vec2i(other.x, other.y) { }
Vec2i::Vec2i(const Vec3i& other) : Vec2i(other.x, other.y) { }
Vec2i::Vec2i(const int x) : Vec2i(x, x) {}
Vec2i::Vec2i() : Vec2i(0, 0) {}