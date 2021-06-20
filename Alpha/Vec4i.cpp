#include "Vec4i.h"
#include "Vec3i.h"
#include "Vec2i.h"

Vec4i::Vec4i(const int x, const int y, const int z, const int w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4i::Vec4i(const Vec2i& other, const int z, const int w) : Vec4i(other.x, other.y, z, w) { }
Vec4i::Vec4i(const Vec3i& other, const int w) : Vec4i(other.x, other.y, other.z, w) { }
Vec4i::Vec4i(const Vec4i& other) : Vec4i(other.x, other.y, other.z, other.w) { }
Vec4i::Vec4i(const int x) : Vec4i(x, x, x, x) {}
Vec4i::Vec4i() : Vec4i(0.0f, 0.0f, 0.0f, 0.0f) {}

int Vec4i::Length() { return sqrt(x * x + y * y + z * z + w * w); }