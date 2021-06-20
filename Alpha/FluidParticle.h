#pragma once
#include "Vec3.h"

class FluidParticle {

private:
	Vec3 m_position;

public:
	FluidParticle(Vec3& t_position) {
		m_position = t_position;
	}
};