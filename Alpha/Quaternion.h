#pragma once

struct Vec3;
struct Mat4;

/*Structure used to represent a quaternion and common operations associated with it*/
struct Quaternion {
	float w, x, y, z;

	Quaternion(float w, float x, float y, float z);
	Quaternion(float w);
	Quaternion();

	float Length() const;
	Mat4 ConvertToMat4();

	static Quaternion Identity();

	static Quaternion Rotation(float angle, Vec3 axis);
	static Quaternion Conjugate(const Quaternion& quaternion);
	static Quaternion Normalize(const Quaternion& quaternion);
	static float Dot(const Quaternion& left, const Quaternion& right);
	static Quaternion Slerp(const Quaternion& v0, const Quaternion& v1, const double t);
	static Vec3 RotatePoint(const Quaternion& rotation, const Vec3& point);
	static Vec3 ConvertToDirection(const Quaternion& rotation);
	static Quaternion RotationBetween(Vec3& vec1, Vec3& vec2);
	static Quaternion RotationBetween3D(Vec3& vec1, Vec3& vec2);

	inline Quaternion operator-() { return Quaternion(-w, -x, -y, -z); }
	inline Quaternion& operator+=(const Quaternion& right) {
		this->w += right.w;
		this->x += right.x;
		this->y += right.y;
		this->z += right.z;
		return *this;
	}
	inline Quaternion& operator-=(const Quaternion& right) {
		this->w -= right.w;
		this->x -= right.x;
		this->y -= right.y;
		this->z -= right.z;
		return *this;
	}
	inline Quaternion& operator*=(const float right) {
		this->w *= right;
		this->x *= right;
		this->y *= right;
		this->z *= right;
		return *this;
	}
	inline Quaternion& operator/=(const float right) {
		this->w /= right;
		this->x /= right;
		this->y /= right;
		this->z /= right;
		return *this;
	}
	
	inline friend Quaternion operator+(Quaternion a, const Quaternion& b) { return a += b; }
	inline friend Quaternion operator-(Quaternion a, const Quaternion& b) { return a -= b; }
	
	inline friend Quaternion operator*(Quaternion a, const float b) { return a *= b; }
	inline friend Quaternion operator*(const float b, Quaternion a) { return a *= b; }
	
	inline friend Quaternion operator/(Quaternion a, const float b) { return a /= b; }
	
	inline friend Quaternion operator*(const Quaternion& a, const Quaternion& b) {
		float w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
		float x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
		float y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
		float z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
		return Quaternion(w, x, y, z);
	}

	inline friend bool operator==(const Quaternion& a, const Quaternion& b) { return (a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w); }
	inline friend bool operator!=(const Quaternion& a, const Quaternion& b) { return !(a == b); }
};