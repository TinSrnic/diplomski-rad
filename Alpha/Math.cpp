#include "Math.h"

float Math::BarryCentric(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec2& pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float Math::Min(float t_first, float t_second) { return t_first < t_second ? t_first : t_second; }
float Math::Max(float t_first, float t_second) { return t_first > t_second ? t_first : t_second; }

void Math::SetRandomSeed(unsigned int seed) { srand(seed); }
float Math::RandomFloat(float t_min, float t_max) {
	return t_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (t_max - t_min)));
}