#pragma once
#include "BoundedShape.h"
#include "Vec2.h"

/*Class used to model a bounded rectangle. 
It provides fuctions to check whether a position is inside it*/
class BoundedRectangle : public BoundedShape {

private:
	float m_width;
	float m_height;

public:
	BoundedRectangle(const Vec2& t_position, float t_width, float t_height) : BoundedShape(t_position) {
		m_width = t_width;
		m_height = t_height;
	}

	bool IsInObject(const Vec2& position) {
		if (position.x >= m_position.x && position.x <= m_position.x + m_width
			&& position.y >= m_position.y && position.y <= m_position.y + m_height) return true;
		else return false;
	}
};
