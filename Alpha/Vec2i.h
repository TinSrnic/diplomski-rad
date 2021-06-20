#pragma once
#include <ostream>

struct Vec3i;

/*Structure used to represent a 2D vector of integers*/
struct Vec2i {
	int x, y;

	Vec2i(const int x, const int y);
	Vec2i(const Vec2i& other);
	Vec2i(const Vec3i& other);
	Vec2i(const int x);
	Vec2i();
	
	inline Vec2i operator-() const { return Vec2i(-x, -y); }
	inline Vec2i& operator+=(const Vec2i& right) {
		this->x += right.x;
		this->y += right.y;
		return *this;
	}
	inline Vec2i& operator-=(const Vec2i& right) {
		this->x -= right.x;
		this->y -= right.y;
		return *this;
	}
	inline Vec2i& operator*=(const int right) {
		this->x -= right;
		this->y -= right;
		return *this;
	}
	
	inline friend Vec2i operator+(Vec2i a, const Vec2i& b) { a += b; }
	inline friend Vec2i operator-(Vec2i a, const Vec2i& b) { a -= b; }
	
	inline friend Vec2i operator*(Vec2i a, const int b) { return a *= b; }
	inline friend Vec2i operator*(const int b, Vec2i a) { return a *= b; }
	
	inline friend bool operator==(const Vec2i& a, const Vec2i& b) { return (a.x == b.x && a.y == b.y); }
	inline friend bool operator!=(const Vec2i& a, const Vec2i& b) { return !(a == b); }
	
	inline friend std::ostream& operator<<(std::ostream& os, const Vec2i& vec) {
		os << "Vec2: (" << vec.x << ", " << vec.y << ")";
		return os;
	}

	//TODO: check this shit out
	struct Vec2MapCompare {
		bool operator() (const Vec2i& a, const Vec2i& b) const {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		}
	};
};