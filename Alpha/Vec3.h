#pragma once
#include <math.h>
#include <ostream>

struct Vec2;
struct Vec4;

/*Structure used to represent a 3D vector*/
struct Vec3 {

	float x, y, z;

	Vec3(const float x, const float y, const float z);
	Vec3(const Vec2& other, const float z);
	Vec3(const Vec3& other);
	Vec3(const Vec4& other);
	Vec3(const float x);
	Vec3();

	float Length();
	static Vec3 Cross(const Vec3& first, const Vec3& second);
	static float Dot(const Vec3& first, const Vec3& second);
	static Vec3 Normalize(const Vec3& vec);
	static Vec3 ClampMagnitude(Vec3& vec, float min, float max);

	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
	inline Vec3& operator+=(const Vec3& right) {
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		return *this;
	}
	inline Vec3& operator-=(const Vec3& right) {
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		return *this;
	}
	inline Vec3& operator*=(const float right) {
		this->x *= right;
		this->y *= right;
		this->z *= right;
		return *this;
	}
	inline Vec3& operator/=(const float right) {
		this->x /= right;
		this->y /= right;
		this->z /= right;
		return *this;
	}
	
	inline friend Vec3 operator+(Vec3 a, const Vec3& b) { return a += b; }
	inline friend Vec3 operator-(Vec3 a, const Vec3& b) { return a -= b; }
	
	inline friend Vec3 operator*(Vec3 a, const float b) { return a *= b; }
	inline friend Vec3 operator*(const float b, Vec3 a) { return a *= b; }
	
	inline friend Vec3 operator/(Vec3 a, const float b) { return a /= b; }

	inline friend bool operator==(const Vec3& a, const Vec3& b) { return (a.x == b.x && a.y == b.y && a.z == b.z); }
	inline friend bool operator!=(const Vec3& a, const Vec3& b) { return !(a == b); }

	inline friend std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
		os << "Vec3: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

};