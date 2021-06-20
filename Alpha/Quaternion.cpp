#include "Quaternion.h"
#include "Vec3.h"
#include "Mat4.h"

Quaternion::Quaternion(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}
Quaternion::Quaternion(float w) : Quaternion(w, w, w, w) { }
Quaternion::Quaternion() : Quaternion(0.0f, 0.0f, 0.0f, 0.0f) { }

float Quaternion::Length() const { return sqrt(w * w + x * x + y * y + z * z); }
Mat4 Quaternion::ConvertToMat4() {
	Quaternion normalized = Normalize(*this);
	float w = normalized.w;
	float x = normalized.x;
	float y = normalized.y;
	float z = normalized.z;
	Mat4 result;

	//www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
	result[0] = 1 - 2 * y * y - 2 * z * z;
	result[1] = 2 * x * y + 2 * z * w;
	result[2] = 2 * x * z - 2 * y * w;
	result[3] = 0;
	result[4] = 2 * x * y - 2 * z * w;
	result[5] = 1 - 2 * x * x - 2 * z * z;
	result[6] = 2 * y * z + 2 * x * w;
	result[7] = 0;
	result[8] = 2 * x * z + 2 * y * w;
	result[9] = 2 * y * z - 2 * x * w;
	result[10] = 1 - 2 * x * x - 2 * y * y;
	result[11] = 0;
	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	result[15] = 1;

	return result;
}

Quaternion Quaternion::Identity() { return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); }

Quaternion Quaternion::Rotation(float angle, Vec3 axis) {
	//en.wikipedia.org/wiki/Quaternions_and_spatial_rotation
	axis = Vec3::Normalize(axis);
	float factor = sin((float)(angle * M_PI / 180.0f) / 2);

	float w = cos((float)(angle * M_PI / 180.0f) / 2);
	float x = axis.x * factor;
	float y = axis.y * factor;
	float z = axis.z * factor;

	return Quaternion(w, x, y, z);
}
Quaternion Quaternion::Conjugate(const Quaternion& quaternion) { return Quaternion(quaternion.w, -quaternion.x, -quaternion.y, -quaternion.z); }
Quaternion Quaternion::Normalize(const Quaternion& quaternion) {
	return (quaternion / quaternion.Length());
}
float Quaternion::Dot(const Quaternion& left, const Quaternion& right) { return left.w * right.w + left.x * right.x + left.y * right.y + left.z * right.z; }
Quaternion Quaternion::Slerp(const Quaternion& start, const Quaternion& end, const double t) {
	//stackoverflow.com/questions/21513637/dot-product-of-two-quaternion-rotations
	// Only unit quaternions are valid rotations.
	// Normalize to avoid undefined behavior.
	Quaternion v0 = Normalize(start);
	Quaternion v1 = Normalize(end);

	// Compute the cosine of the angle between the two vectors.
	float dot = Dot(v0, v1);
	const double DOT_THRESHOLD = 0.9995;
	if (fabs(dot) > DOT_THRESHOLD) {
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.

		Quaternion result = v0 + (float)t * (v1 - v0);
		return Normalize(result);
	}

	// If the dot product is negative, the quaternions
	// have opposite handed-ness and slerp won't take
	// the shorter path. Fix by reversing one quaternion.
	if (dot < 0.0f) {
		v1 = -1 * v1;
		dot = -dot;
	}

	//Clamp(dot, -1, 1); Robustness: Stay within domain of acos()
	if (dot < -1) dot = -1.0f;
	else if (dot > 1) dot = 1.0f;

	double theta_0 = acos(dot);  // theta_0 = angle between input vectors
	double theta = theta_0 * t;    // theta = angle between v0 and result 

	Quaternion v2 = v1 - (v0 * dot);
	v2 = Normalize(v2);              // { v0, v2 } is now an orthonormal basis

	return v0 * (float)cos(theta) + v2 * (float)sin(theta);
}

Vec3 Quaternion::RotatePoint(const Quaternion& rotation, const Vec3& v) {
    //gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
	Vec3 u = Vec3(rotation.x, rotation.y, rotation.z);
	float s = rotation.w;

	Vec3 res = 2 * Vec3::Dot(u, v) * u + (s * s - Vec3::Dot(u, u)) * v + 2 * s * Vec3::Cross(u, v);
	return res;
}

Vec3 Quaternion::ConvertToDirection(const Quaternion& rotation) {
	Vec3 result = Vec3(0, 0, 0);
	result.x = 2 * (rotation.x * rotation.y - rotation.w * rotation.z);
	result.y = 1 - 2 * (rotation.x * rotation.x + rotation.z * rotation.z);
	result.z = 2 * (rotation.y * rotation.z + rotation.w * rotation.x);

	return result;
}

Quaternion Quaternion::RotationBetween(Vec3& vec1, Vec3& vec2) {
	Quaternion q;
	float dotProduct = Vec3::Dot(Vec3::Normalize(vec1), Vec3::Normalize(vec2));
	if (dotProduct > 0.999) {
		return Quaternion::Identity();
	} else if (dotProduct < -0.999) {
		return Quaternion::Rotation(180, Vec3(0, 0, 1));
	}

	Vec3 a = Vec3::Cross(vec1, vec2);
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	q.w = sqrt(pow(vec1.Length(), 2) * pow(vec2.Length(), 2)) + Vec3::Dot(vec1, vec2);

	return Quaternion::Normalize(q);
}

Quaternion Quaternion::RotationBetween3D(Vec3& vec1, Vec3& vec2) {
	Quaternion q;
	float dotProduct = Vec3::Dot(Vec3::Normalize(vec1), Vec3::Normalize(vec2));
	if (dotProduct > 0.999) {
		return Quaternion::Identity();
	}
	else if (dotProduct < -0.999) {
		return Quaternion::Rotation(180, Vec3(-vec1.y, vec1.x, vec1.z));
	}

	Vec3 a = Vec3::Cross(vec1, vec2);
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	q.w = sqrt(pow(vec1.Length(), 2) * pow(vec2.Length(), 2)) + Vec3::Dot(vec1, vec2);

	return Quaternion::Normalize(q);
}