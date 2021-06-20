#include "Mat4.h"
#include "Quaternion.h"

Mat4::Mat4() {
	for (int x = 0; x < 16; x++) m_elements[x] = 0.0f;
}

//www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
Quaternion Mat4::ConvertToQuaternion() {
	float qw, qx, qy, qz;
	float tr = m_elements[0] + m_elements[5] + m_elements[10];

	if (tr > 0) {
		float S = sqrt(tr + 1.0f) * 2.0f; // S=4*qw 
		qw = 0.25f * S;
		qx = (m_elements[9] - m_elements[6]) / S;
		qy = (m_elements[2] - m_elements[8]) / S;
		qz = (m_elements[4] - m_elements[1]) / S;
	} else if ((m_elements[0] > m_elements[5])&(m_elements[0] > m_elements[10])) {
		float S = sqrt(1.0f + m_elements[0] - m_elements[5] - m_elements[10]) * 2.0f; // S=4*qx 
		qw = (m_elements[9] - m_elements[6]) / S;
		qx = 0.25f * S;
		qy = (m_elements[1] + m_elements[4]) / S;
		qz = (m_elements[2] + m_elements[8]) / S;
	} else if (m_elements[5] > m_elements[10]) {
		float S = sqrt(1.0f + m_elements[5] - m_elements[0] - m_elements[10]) * 2.0f; // S=4*qy
		qw = (m_elements[2] - m_elements[8]) / S;
		qx = (m_elements[1] + m_elements[4]) / S;
		qy = 0.25f * S;
		qz = (m_elements[6] + m_elements[9]) / S;
	} else {
		float S = sqrt(1.0f + m_elements[10] - m_elements[0] - m_elements[5]) * 2.0f; // S=4*qz
		qw = (m_elements[4] - m_elements[1]) / S;
		qx = (m_elements[2] + m_elements[8]) / S;
		qy = (m_elements[6] + m_elements[9]) / S;
		qz = 0.25f * S;
	}

	return Quaternion(qw, -qx, -qy, -qz);
}

Mat4 Mat4::Identity() {
	Mat4 mat = Mat4();
	for (int x = 0; x < 4; x++) mat.m_elements[x * 4 + x] = 1.0f;
	return mat;
}

Mat4 Mat4::Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
	Mat4 orthographic = Mat4();

	orthographic[0] = 2 / (right - left);
	orthographic[1] = 0;
	orthographic[2] = 0;
	orthographic[3] = 0;
	orthographic[4] = 0;
	orthographic[5] = 2 / (top - bottom);
	orthographic[6] = 0;
	orthographic[7] = 0;
	orthographic[8] = 0;
	orthographic[9] = 0;
	orthographic[10] = -2 / (farPlane - nearPlane);
	orthographic[11] = 0;
	orthographic[12] = -1 * (right + left) / (right - left);
	orthographic[13] = -1 * (top + bottom) / (top - bottom);
	orthographic[14] = -1 * (farPlane + nearPlane) / (farPlane - nearPlane);
	orthographic[15] = 1;

	return orthographic;
}
Mat4 Mat4::Perspective(float FOV, float aspect, float nearPlane, float farPlane) {
	Mat4 perspective = Mat4();
	float FOVrad = (float)(FOV * M_PI / 180.0f);

	perspective[0] = 1 / (aspect * tan(FOVrad / 2));
	perspective[1] = 0;
	perspective[2] = 0;
	perspective[3] = 0;
	perspective[4] = 0;
	perspective[5] = 1 / (tan(FOVrad / 2));
	perspective[6] = 0;
	perspective[7] = 0;
	perspective[8] = 0;
	perspective[9] = 0;
	perspective[10] = -1 * (farPlane + nearPlane) / (farPlane - nearPlane);
	perspective[11] = -1;
	perspective[12] = 0;
	perspective[13] = 0;
	perspective[14] = -1 * (2 * farPlane * nearPlane) / (farPlane - nearPlane);
	perspective[15] = 0;

	return perspective;
}

Mat4 Mat4::Translation(const Vec3& translation) {
	Mat4 trans = Mat4();

	float x = translation.x;
	float y = translation.y;
	float z = translation.z;

	trans[0] = 1;
	trans[1] = 0;
	trans[2] = 0;
	trans[3] = 0;
	trans[4] = 0;
	trans[5] = 1;
	trans[6] = 0;
	trans[7] = 0;
	trans[8] = 0;
	trans[9] = 0;
	trans[10] = 1;
	trans[11] = 0;
	trans[12] = x;
	trans[13] = y;
	trans[14] = z;
	trans[15] = 1;

	return trans;
}
Mat4 Mat4::Rotation(float angle, const Vec3& axisVec) {
	Mat4 rot = Mat4();
	Vec3 axis = Vec3::Normalize(axisVec);
	float c = (float)(cos(angle * M_PI / 180.0));
	float s = (float)(sin(angle * M_PI / 180.0));
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	rot[0] = x * x * (1 - c) + c;
	rot[1] = x * y * (1 - c) + z * s;
	rot[2] = x * z * (1 - c) - y * s;
	rot[3] = 0;
	rot[4] = x * y * (1 - c) - z * s;
	rot[5] = y * y * (1 - c) + c;
	rot[6] = y * z * (1 - c) + x * s;
	rot[7] = 0;
	rot[8] = x * z * (1 - c) + y * s;
	rot[9] = y * z * (1 - c) - x * s;
	rot[10] = z * z * (1 - c) + c;
	rot[11] = 0;
	rot[12] = 0;
	rot[13] = 0;
	rot[14] = 0;
	rot[15] = 1;

	return rot;
}
Mat4 Mat4::Scale(const Vec3& scale) {
	Mat4 scaleMat = Mat4();
	scaleMat[0] = scale.x;
	scaleMat[5] = scale.y;
	scaleMat[10] = scale.z;
	scaleMat[15] = 1;
	return scaleMat;
}

Mat4 Mat4::Invert(const Mat4& m) {
	Mat4 inv = Mat4();
	Mat4 invOut = Mat4();
	float det;
	int x;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0) return Mat4::Identity();

	det = 1.0f / det;

	for (x = 0; x < 16; x++) invOut[x] = inv[x] * det;

	return invOut;
}
Mat4 Mat4::Transpose(const Mat4& in) {
	Mat4 out = Mat4();

	out[0] = in[0];
	out[1] = in[4];
	out[2] = in[8];
	out[3] = in[12];
	out[4] = in[1];
	out[5] = in[5];
	out[6] = in[9];
	out[7] = in[13];
	out[8] = in[2];
	out[9] = in[6];
	out[10] = in[10];
	out[11] = in[14];
	out[12] = in[3];
	out[13] = in[7];
	out[14] = in[11];
	out[15] = in[15];

	return out;
}

Mat4 Mat4::RotationFromDirection(const Vec3& t_direction, const Vec3& t_up) {
	float dotProduct = Vec3::Dot(Vec3::Normalize(t_direction), Vec3::Normalize(t_up));
	if (dotProduct > 0.999) {
		return Mat4::Identity();
	} else if (dotProduct < -0.999) {
		return Mat4::Rotation(180, Vec3(-t_direction.y, t_direction.x, t_direction.z));
	}

	Mat4 result = Mat4();
	Vec3 normalizedDirection = Vec3::Normalize(t_direction);

	Vec3 xAxis = Vec3::Cross(t_up, normalizedDirection);
	xAxis = Vec3::Normalize(xAxis);

	Vec3 yAxis = Vec3::Cross(normalizedDirection, xAxis);
	yAxis = Vec3::Normalize(yAxis);

	result[0] = xAxis.x;
	result[1] = yAxis.x;
	result[2] = normalizedDirection.x;
	result[3] = 0;
	result[4] = xAxis.y;
	result[5] = yAxis.y;
	result[6] = normalizedDirection.y;
	result[7] = 0;
	result[8] = xAxis.z;
	result[9] = yAxis.z;
	result[10] = normalizedDirection.z;
	result[11] = 0;
	result[12] = 0;
	result[13] = 0;
	result[14] = 0;
	result[15] = 1;

	//the second rotation is neccessary because the defult view is along the -z axis and not the +z
	return Mat4::Rotation(180.0f, Vec3(0.0f, 1.0f, 0.0f)) * result; 
}