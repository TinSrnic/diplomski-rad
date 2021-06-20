#pragma once
#include "Math.h"

class Line {

private:
	Vec3 m_startPoint;
	Vec3 m_endPoint;
	Vec4 m_startColor;
	Vec4 m_endColor;

public:
	Line(const Vec3& t_startPoint, const Vec3& t_endPoint, const Vec4& t_startColor, const Vec4& t_endColor) {
		m_startPoint = t_startPoint;
		m_endPoint = t_endPoint;
		m_startColor = t_startColor;
		m_endColor = t_endColor;
	}

	Vec3 GetStartPoint() { return m_startPoint; }
	Vec3 GetEndPoint() { return m_endPoint; }
	Vec4 GetStartColor() { return m_startColor; }
	Vec4 GetEndColor() { return m_endColor; }

	void SetStartPoint(const Vec3& t_startPoint) { m_startPoint = t_startPoint; }
	void SetEndPoint(const Vec3& t_endPoint) { m_endPoint = t_endPoint; }
	void SetStartColor(const Vec4& t_startColor) { m_startColor = t_startColor; }
	void SetEndColor(const Vec4& t_endColor) { m_endColor = t_endColor; }
};