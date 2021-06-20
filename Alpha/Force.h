#pragma once
#include "Math.h"

struct Force {
	Vec3 source;
	Vec3 direction;

	Force(const Vec3& t_source, const Vec3& t_direction) {
		source = t_source;
		direction = t_direction;
	}
	Force() {
		source = Vec3(0.0f);
		direction = Vec3(0.0f);
	}

	void AddForce(const Force& other) {

	}
};