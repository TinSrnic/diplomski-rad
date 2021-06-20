#pragma once
#include "Group.h"
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "Input.h"

class Scrollable : public Group {

private:


public:
	Scrollable(const Vec3& t_position, const Vec3& t_scale) 
		: Group(BoundedGUI(GUI(TexturedModel(ModelLoader::LoadModel("plane.obj"), Texture(TextureLoader::LoadTexture("menu.png"), 1, 0.01f, 10.0f), 0), 0, "someName", t_position, Quaternion::Identity(), t_scale, false, "bounce"), std::make_shared<BoundedRectangle>(BoundedRectangle(t_position, t_scale.x, t_scale.y)))) {
	}

	void Update() {
		Vec2 mouseScroll = Input::GetMouseScroll();
		for each(std::pair<std::shared_ptr<Clickable>, Vec3> controlPosition in m_controls) {
			Vec3 newPosition = controlPosition.first->GetPosition() + Vec3(mouseScroll * SCROLL_SPEED, 0.0f);
			controlPosition.first->SetPosition(newPosition);
			controlPosition.second = newPosition;
			controlPosition.first->ChangedPosition();
		}
	}
};