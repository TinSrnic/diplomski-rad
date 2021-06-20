#pragma once
#include <ostream>

struct Vec2;
struct Vec3;

/*Structure used to represent a 4D vector*/
struct Vec4 {
	
	float x, y, z, w;
	
	Vec4(const float x, const float y, const float z, const float w);
	Vec4(const Vec2& other, const float z, const float w);
	Vec4(const Vec3& other, const float w);
	Vec4(const Vec4& other);
	Vec4(const float x);
	Vec4();

	float Length();
	static Vec4 Normalize(const Vec4& vec);

	inline Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }
	inline Vec4& operator+=(const Vec4& right) {
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		this->w += right.w;
		return *this;
	}
	inline Vec4& operator-=(const Vec4& right) {
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		this->w -= right.w;
		return *this;
	}
	inline Vec4& operator*=(const float right) {
		this->x *= right;
		this->y *= right;
		this->z *= right;
		this->w *= right;
		return *this;
	}
	inline Vec4& operator/=(const float right) {
		this->x /= right;
		this->y /= right;
		this->z /= right;
		this->w /= right;
		return *this;
	}
	
	inline friend Vec4 operator+(Vec4 a, const Vec4& b) { return a += b; }
	inline friend Vec4 operator-(Vec4 a, const Vec4& b) { return a -= b; }
	
	inline friend Vec4 operator*(Vec4 a, const float b) { return a *= b; }
	inline friend Vec4 operator*(const float b, Vec4 a) { return a *= b; }
	
	inline friend Vec4 operator/(Vec4 a, const float b) { return a /= b; }
	
	inline friend bool operator==(const Vec4& a, const Vec4& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
	inline friend bool operator!=(const Vec4& a, const Vec4& b) { return !(a == b); }
	
	inline friend std::ostream& operator<<(std::ostream& os, const Vec4& vec) {
		os << "Vec4: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return os;
	}
};