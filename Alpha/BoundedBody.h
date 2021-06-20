#pragma once
#include <vector>
#include "Math.h"

class BoundedBody {

protected:
	std::vector<Vec3> m_vertices;

	Vec3 m_position;
	Vec3 m_scale;
	Quaternion m_rotation;

public:
	BoundedBody(const Vec3& t_position, const Vec3& t_scale, const Quaternion& t_rotation) {
		m_vertices = std::vector<Vec3>();

		m_position = t_position;
		m_scale = t_scale;
		m_rotation = t_rotation;
	}
	BoundedBody(const Vec3& t_position, const Vec3& t_scale) {
		m_position = t_position;
		m_scale = t_scale;
		m_rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	}

	virtual Vec3 Support(const Vec3& t_direction) const {
		Vec3 res = m_vertices[0];
		res.x *= m_scale.x;
		res.y *= m_scale.y;
		res.z *= m_scale.z;
		res = Quaternion::RotatePoint(m_rotation, res);
		float max = Vec3::Dot(res, t_direction);

		for (int i = 1; i < m_vertices.size(); ++i) {
			Vec3 finalVertex = m_vertices[i];
			finalVertex.x *= m_scale.x;
			finalVertex.y *= m_scale.y;
			finalVertex.z *= m_scale.z;
			finalVertex = Quaternion::RotatePoint(m_rotation, finalVertex);
			float dot = Vec3::Dot(finalVertex, t_direction);
			if (dot > max) {
				max = dot;
				res = finalVertex;
			}
		}

		return res + m_position;
	}

	Vec3 GetPosition() const { return m_position; }
	Vec3 GetScale() const { return m_scale; }
	Quaternion GetRotation() const { return m_rotation; }

	void SetPosition(const Vec3& t_position) { m_position = t_position; }
	void SetScale(const Vec3& t_scale) { m_scale = t_scale; }
	void SetRotation(const Quaternion& t_rotation) { m_rotation = t_rotation; }
};