#pragma once
#include <ostream>

struct Vec3;
struct Vec4;

/*Structure used to represent a 2D vector*/
struct Vec2 {
	float x, y;

	Vec2(const float x, const float y);
	Vec2(const Vec2& other);
	Vec2(const Vec3& other);
	Vec2(const Vec4& other);
	Vec2(const float x);
	Vec2();

	float Length();
	static Vec2 Normalize(const Vec2& vec);
	static float Dot(const Vec2& first, const Vec2& second);

	inline Vec2 operator-() const { return Vec2(-x, -y); }
	inline Vec2& operator+=(const Vec2& right) {
		this->x += right.x;
		this->y += right.y;
		return *this;
	}
	inline Vec2& operator-=(const Vec2& right) {
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}
	inline Vec2& operator*=(const float right) {
		this->x *= right;
		this->y *= right;
		return *this;
	}
	inline Vec2& operator/=(const float right) {
		this->x /= right;
		this->y /= right;
		return *this;
	}
	
	inline friend Vec2 operator+(Vec2 a, const Vec2& b) { return a += b; }
	inline friend Vec2 operator-(Vec2 a, const Vec2& b) { return a -= b; }
	
	inline friend Vec2 operator*(Vec2 a, const float b) { return a *= b; }
	inline friend Vec2 operator*(const float b, Vec2 a) { return a *= b; }
	
	inline friend Vec2 operator/(Vec2 a, const float b) { return a /= b; }
	
	inline friend bool operator==(const Vec2& a, const Vec2& b) { return (a.x == b.x && a.y == b.y); }
	inline friend bool operator!=(const Vec2& a, const Vec2& b) { return !(a == b); }
	
	inline friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
		os << "Vec2: (" << vec.x << ", " << vec.y << ")";
		return os;
	}

	//TODO: check this shit out
	struct Vec2MapCompare {
		bool operator() (const Vec2& a, const Vec2& b) const {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		}
	};
};