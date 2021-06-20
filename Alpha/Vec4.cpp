#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"

Vec4::Vec4(const float x, const float y, const float z, const float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4::Vec4(const Vec2& other, const float z, const float w) : Vec4(other.x, other.y, z, w) { }
Vec4::Vec4(const Vec3& other, const float w) : Vec4(other.x, other.y, other.z, w) { }
Vec4::Vec4(const Vec4& other) : Vec4(other.x, other.y, other.z, other.w) { }
Vec4::Vec4(const float x) : Vec4(x, x, x, x) {}
Vec4::Vec4() : Vec4(0.0f, 0.0f, 0.0f, 0.0f) {}

float Vec4::Length() { return sqrt(x * x + y * y + z * z + w * w); }
Vec4 Vec4::Normalize(const Vec4& vec) {
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	return Vec4(vec.x / length, vec.y / length, vec.z / length, vec.w / length);
}