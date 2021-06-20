#pragma once
#include "Clickable.h"
#include "Text.h"
#include "GameObjects.h"
#include "ModelLoader.h"
#include "TextureLoader.h"

class Label : public Clickable {

private:
	std::shared_ptr<Text> m_text;

public:
	Label(const std::shared_ptr<Text>& t_text, const Vec3& t_position) 
		: Clickable(BoundedGUI(GUI(TexturedModel(ModelLoader::LoadModel("plane"), Texture(TextureLoader::LoadTexture("labelBackground.png"), 1, 0.01f, 10.0f), 0), 0, "label1", t_position, Quaternion::Identity(), Vec3(1.0f), false, "bounce"), std::make_shared<BoundedRectangle>(BoundedRectangle(t_position, 1.0f, 1.0f))), std::function<void(Clickable*)>()) {
		m_text = t_text;
		m_text->SetPosition(t_position + Vec3(-1.0f, 0.8f, 2.0f));
		this->SetScale(Vec3(1.0f, 0.3f, 1.0f));
	}

	std::shared_ptr<Text> GetText() { return m_text; }
	void SetText(const std::shared_ptr<Text> t_text) { m_text = t_text; }

	void SetPosition(const Vec3& t_position) {
		this->Clickable::SetPosition(t_position);
		m_text->SetPosition(t_position + Vec3(0.0f, 0.0f, 2.0f));
	}

	void SetVisible(bool t_visible) override {
		this->GUI::SetVisible(t_visible);
	}
};