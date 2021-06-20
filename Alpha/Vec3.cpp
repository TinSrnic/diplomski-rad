#include "Vec3.h"
#include "Vec4.h"
#include "Vec2.h"
#include <algorithm>

Vec3::Vec3(const float x, const float y, const float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3::Vec3(const Vec2& other, const float z) : Vec3(other.x, other.y, z) { }
Vec3::Vec3(const Vec3& other) : Vec3(other.x, other.y, other.z) { }
Vec3::Vec3(const Vec4& other) : Vec3(other.x, other.y, other.z) { }
Vec3::Vec3(const float x) : Vec3(x, x, x) {}
Vec3::Vec3() : Vec3(0.0f, 0.0f, 0.0f) {}

float Vec3::Length() { return sqrt(x * x + y * y + z * z); }
Vec3 Vec3::Cross(const Vec3& first, const Vec3& second) {
	float x = first.y * second.z - first.z * second.y;
	float y = first.z * second.x - first.x * second.z;
	float z = first.x * second.y - first.y * second.x;

	return Vec3(x, y, z);
}
float Vec3::Dot(const Vec3& first, const Vec3& second) {
	return (first.x * second.x + first.y * second.y + first.z * second.z);
}
Vec3 Vec3::Normalize(const Vec3& vec) {
	float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	return Vec3(vec.x / length, vec.y / length, vec.z / length);
}
Vec3 Vec3::ClampMagnitude(Vec3& vec, float min, float max) {
	float length = vec.Length();
	return Vec3::Normalize(vec) * std::clamp(length, min, max);
}