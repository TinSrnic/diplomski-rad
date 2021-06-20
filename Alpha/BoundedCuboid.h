#pragma once
#include <limits>
#include "BoundedBody.h"

//position is bottom, right, towards screen
class BoundedCuboid : public BoundedBody {

private:

public:
	BoundedCuboid(const Vec3& t_position, const Vec3& t_scale) : BoundedBody(t_position, t_scale) {
		m_vertices = std::vector<Vec3>();
		m_vertices.push_back(Vec3(-1.0f, -1.0f, -1.0f));
		m_vertices.push_back(Vec3(1.0f, -1.0f, -1.0f));
		m_vertices.push_back(Vec3(1.0f, -1.0f, 1.0f));
		m_vertices.push_back(Vec3(-1.0f, -1.0f, 1.0f));
		m_vertices.push_back(Vec3(-1.0f, 1.0f, -1.0f));
		m_vertices.push_back(Vec3(1.0f, 1.0f, -1.0f));
		m_vertices.push_back(Vec3(1.0f, 1.0f, 1.0f));
		m_vertices.push_back(Vec3(-1.0f, 1.0f, 1.0f));
	}
};