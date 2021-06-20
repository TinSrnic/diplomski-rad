#pragma once
#include "GUI.h"
#include "BoundedRectangle.h"

class BoundedGUI : public GUI {

private:
	std::shared_ptr<BoundedShape> m_boundingShape;

public:
	BoundedGUI(const GUI& t_gui, const std::shared_ptr<BoundedShape> t_boundingShape) 
		: GUI(t_gui), m_boundingShape(t_boundingShape) { }

	Vec3 GetPosition() { return this->GUI::GetPosition(); }

	void SetPosition(const Vec3& t_position) {
		this->GUI::SetPosition(t_position);
		m_boundingShape->SetPosition(t_position);
	}

	bool IsInObject(const Vec2& t_position) { return m_boundingShape->IsInObject(t_position); }

};
