#pragma once
#include <ostream>

struct Vec2i;

/*Structure used to represent a 3D vector of integers*/
struct Vec3i {
	int x, y, z;

	Vec3i(const int x, const int y, const int z);
	Vec3i(const Vec2i& other, const int z);
	Vec3i(const Vec3i& other);
	Vec3i(const int x);
	Vec3i();

	inline Vec3i operator-() const { return Vec3i(-x, -y, -z); }
	inline Vec3i& operator+=(const Vec3i& right) {
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		return *this;
	}
	inline Vec3i& operator-=(const Vec3i& right) {
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		return *this;
	}
	inline Vec3i& Vec3i::operator*=(const int right) {
		this->x *= right;
		this->y *= right;
		this->z *= right;
		return *this;
	}
	
	inline friend Vec3i operator+(Vec3i a, const Vec3i& b) { a += b; }
	inline friend Vec3i operator-(Vec3i a, const Vec3i& b) { a -= b; }
	
	inline friend Vec3i operator*(Vec3i a, const int b) { a *= b; }
	inline friend Vec3i operator*(const int b, Vec3i a) { a *= b; }
	
	inline friend std::ostream& operator<<(std::ostream& os, const Vec3i& vec) {
		os << "Vec3i: (" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	//TODO: check this shit out
	struct Vec3iMapCompare {
		bool operator() (const Vec3i& a, const Vec3i& b) const {
			return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z);
		}
	};

};