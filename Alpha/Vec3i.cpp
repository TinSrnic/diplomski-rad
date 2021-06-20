#include "Vec3i.h"
#include "Vec2i.h"

Vec3i::Vec3i(const int x, const int y, const int z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3i::Vec3i(const Vec2i& other, const int z) : Vec3i(other.x, other.y, z) { }
Vec3i::Vec3i(const Vec3i& other) : Vec3i(other.x, other.y, other.z) { }
Vec3i::Vec3i(const int x) : Vec3i(x, x, x) {}
Vec3i::Vec3i() : Vec3i(0, 0, 0) {}