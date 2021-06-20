#pragma once
#include "Math.h"

class BoundedShape {

protected:
	Vec2 m_position;

public:
	BoundedShape(const Vec2& t_position) {
		m_position = t_position; 
	}

	virtual bool IsInObject(const Vec2& position) { return false; }

	Vec2& GetPosition() { return m_position; }
	void SetPosition(const Vec2& t_position) { m_position = t_position; }
};