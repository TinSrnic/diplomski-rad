#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "Vec3.h"
#include "Vec4.h"

struct Quaternion;

/*Structure used to represent a 4x4 matrix and common operations used with it*/
struct Mat4 {
	float m_elements[4 * 4];
	
	Mat4();

	Quaternion ConvertToQuaternion();

	static Mat4 Identity();

	static Mat4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	static Mat4 Perspective(float FOV, float aspect, float nearPlane, float farPlane);

	static Mat4 Translation(const Vec3& translation);
	static Mat4 Rotation(float angle, const Vec3& axisVec);
	static Mat4 Scale(const Vec3& scale);
	
	static Mat4 Invert(const Mat4& m);
	static Mat4 Transpose(const Mat4& in);

	static Mat4 RotationFromDirection(const Vec3& t_direction, const Vec3& t_up = Vec3(0.0f, 1.0f, 0.0f));

	inline float& Mat4::operator[](int index) { return m_elements[index]; }
	inline float Mat4::operator[](int index) const { return m_elements[index]; }
	inline friend Mat4 operator*(const Mat4& left, const Mat4& right) {
		Mat4 mat = Mat4();

		mat[0] = left[0] * right[0] + left[4] * right[1] + left[8] * right[2] + left[12] * right[3];
		mat[1] = left[1] * right[0] + left[5] * right[1] + left[9] * right[2] + left[13] * right[3];
		mat[2] = left[2] * right[0] + left[6] * right[1] + left[10] * right[2] + left[14] * right[3];
		mat[3] = left[3] * right[0] + left[7] * right[1] + left[11] * right[2] + left[15] * right[3];
		mat[4] = left[0] * right[4] + left[4] * right[5] + left[8] * right[6] + left[12] * right[7];
		mat[5] = left[1] * right[4] + left[5] * right[5] + left[9] * right[6] + left[13] * right[7];;
		mat[6] = left[2] * right[4] + left[6] * right[5] + left[10] * right[6] + left[14] * right[7];;
		mat[7] = left[3] * right[4] + left[7] * right[5] + left[11] * right[6] + left[15] * right[7];;
		mat[8] = left[0] * right[8] + left[4] * right[9] + left[8] * right[10] + left[12] * right[11];
		mat[9] = left[1] * right[8] + left[5] * right[9] + left[9] * right[10] + left[13] * right[11];
		mat[10] = left[2] * right[8] + left[6] * right[9] + left[10] * right[10] + left[14] * right[11];
		mat[11] = left[3] * right[8] + left[7] * right[9] + left[11] * right[10] + left[15] * right[11];
		mat[12] = left[0] * right[12] + left[4] * right[13] + left[8] * right[14] + left[12] * right[15];
		mat[13] = left[1] * right[12] + left[5] * right[13] + left[9] * right[14] + left[13] * right[15];
		mat[14] = left[2] * right[12] + left[6] * right[13] + left[10] * right[14] + left[14] * right[15];
		mat[15] = left[3] * right[12] + left[7] * right[13] + left[11] * right[14] + left[15] * right[15];

		return mat;
	}
	inline friend Vec3 operator*(const Mat4& mat, const Vec3& vec) {
		float x = 0, y = 0, z = 0, w = 0;
		x += mat[0] * vec.x;
		x += mat[4] * vec.y;
		x += mat[8] * vec.z;
		x += mat[12] * 1.0f;

		y += mat[1] * vec.x;
		y += mat[5] * vec.y;
		y += mat[9] * vec.z;
		y += mat[13] * 1.0f;

		z += mat[2] * vec.x;
		z += mat[6] * vec.y;
		z += mat[10] * vec.z;
		z += mat[14] * 1.0f;

		return Vec3(x, y, z);
	}
	inline friend Vec4 operator*(const Mat4& mat, const Vec4& vec) {
		float x = 0, y = 0, z = 0, w = 0;
		x += mat[0] * vec.x;
		x += mat[4] * vec.y;
		x += mat[8] * vec.z;
		x += mat[12] * vec.w;

		y += mat[1] * vec.x;
		y += mat[5] * vec.y;
		y += mat[9] * vec.z;
		y += mat[13] * vec.w;

		z += mat[2] * vec.x;
		z += mat[6] * vec.y;
		z += mat[10] * vec.z;
		z += mat[14] * vec.w;

		w += mat[3] * vec.x;
		w += mat[7] * vec.y;
		w += mat[11] * vec.z;
		w += mat[15] * vec.w;

		return Vec4(x, y, z, w);
	}

	
};