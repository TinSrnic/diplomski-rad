#pragma once
#define NOMINMAX

#include "Vec2i.h"
#include "Vec3i.h"
#include "Vec4i.h"

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

#include "Mat4.h"

#include "Quaternion.h"

/*Class holding useful math functions*/
class Math {

public:
	static float BarryCentric(const Vec3& p1, const Vec3& p2, const Vec3& p3, const Vec2& pos);

	static float Min(float t_first, float t_second);
	static float Max(float t_first, float t_second);

	static void SetRandomSeed(unsigned int seed);
	static float RandomFloat(float t_min, float t_max);

};
