#pragma once
#include <ostream>

struct Vec2i;
struct Vec3i;

/*Structure used to represent a 4D vector*/
struct Vec4i {

	int x, y, z, w;

	Vec4i(const int x, const int y, const int z, const int w);
	Vec4i(const Vec2i& other, const int z, const int w);
	Vec4i(const Vec3i& other, const int w);
	Vec4i(const Vec4i& other);
	Vec4i(const int x);
	Vec4i();

	int Length();

	inline Vec4i operator-() const { return Vec4i(-x, -y, -z, -w); }
	inline Vec4i& operator+=(const Vec4i& right) {
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		this->w += right.w;
		return *this;
	}
	inline Vec4i& operator-=(const Vec4i& right) {
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		this->w -= right.w;
		return *this;
	}
	inline Vec4i& operator*=(const int right) {
		this->x *= right;
		this->y *= right;
		this->z *= right;
		this->w *= right;
		return *this;
	}
	inline Vec4i& operator/=(const int right) {
		this->x /= right;
		this->y /= right;
		this->z /= right;
		this->w /= right;
		return *this;
	}

	inline friend Vec4i operator+(Vec4i a, const Vec4i& b) { return a += b; }
	inline friend Vec4i operator-(Vec4i a, const Vec4i& b) { return a -= b; }

	inline friend Vec4i operator*(Vec4i a, const int b) { return a *= b; }
	inline friend Vec4i operator*(const int b, Vec4i a) { return a *= b; }

	inline friend Vec4i operator/(Vec4i a, const int b) { return a /= b; }

	inline friend bool operator==(const Vec4i& a, const Vec4i& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
	inline friend bool operator!=(const Vec4i& a, const Vec4i& b) { return !(a == b); }

	inline friend std::ostream& operator<<(std::ostream& os, const Vec4i& vec) {
		os << "Vec4i: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}
};