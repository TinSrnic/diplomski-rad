#include "Vec2.h"
#include "Vec4.h"
#include "Vec3.h"

Vec2::Vec2(const float x, const float y) {
	this->x = x;
	this->y = y;
}
Vec2::Vec2(const Vec2& other) : Vec2(other.x, other.y) { }
Vec2::Vec2(const Vec3& other) : Vec2(other.x, other.y) { }
Vec2::Vec2(const Vec4& other) : Vec2(other.x, other.y) { }
Vec2::Vec2(const float x) : Vec2(x, x) {}
Vec2::Vec2() : Vec2(0.0f, 0.0f) {}

float Vec2::Length() { return sqrt(x * x + y * y); }
Vec2 Vec2::Normalize(const Vec2& vec) {
	float length = sqrt(vec.x * vec.x + vec.y * vec.y);
	return Vec2(vec.x / length, vec.y / length);
}
float Vec2::Dot(const Vec2& first, const Vec2& second) {
	return (first.x * second.x + first.y * second.y);
}