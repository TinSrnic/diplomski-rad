#pragma once
#include "Group.h"
#include "Entity.h"
#include "Label.h"
#include "GameObjects.h"

class EntityDisplay : public Group {
	
private:
	std::shared_ptr<Clickable> m_guiEntity;
	std::shared_ptr<Label> m_label;
	std::shared_ptr<Entity> m_spawnedEntity = nullptr;
	float m_currentRotation;
	float m_rotationSpeed;

	float m_scale = 1.0f;

public:
	EntityDisplay(const Vec3& t_position, const std::shared_ptr<Clickable>& t_guiEntity, const float t_rotationSpeed)
		: Group(BoundedGUI(GUI(TexturedModel(ModelLoader::LoadModel("plane.obj"), Texture(TextureLoader::LoadTexture("background.png"), 1, 0.01f, 10.0f), 0), 0, "someName", t_position, Quaternion::Identity(), Vec3(1.0f), false, "bounce"), std::make_shared<BoundedRectangle>(BoundedRectangle(t_position, 1.0f, 1.0f)))) { 
		m_currentRotation = 0.0f;
		m_rotationSpeed = t_rotationSpeed;
		m_guiEntity = t_guiEntity;

		this->SetMouseClickFunction(std::function<void(Clickable*)>(EntityDisplay::SpawnObject));
		this->SetMouseHeldFunction(std::function<void(Clickable*)>(EntityDisplay::HoldObject));
		this->SetMouseReleaseFunction(std::function<void(Clickable*)>(EntityDisplay::ReleaseObject));

		m_label = std::make_shared<Label>(Label(std::make_shared<Text>(Text(FontLoader::LoadFont("candara.fnt", "candara.png"), "stall.obj", this->GetPosition())), this->GetPosition()));

		this->AddControl(m_label, Vec3(0.0f, 0.6f, 20.0f));

		this->AddControl(m_guiEntity, Vec3(0.0f, -0.2f, 10.0f));
		Vec3 dimensions = m_guiEntity->GetRawModel().GetDimensions();
		float maxDimension = 0.0f;
		if (dimensions.x > maxDimension) maxDimension = dimensions.x;
		if (dimensions.y > maxDimension) maxDimension = dimensions.y;
		if (dimensions.z > maxDimension) maxDimension = dimensions.z;

		m_guiEntity->SetScale(Vec3(1 / maxDimension));
	}

	void Update() override {
		m_currentRotation += m_rotationSpeed;
		while (m_currentRotation >= 360.0f) m_currentRotation -= 360.0f;
		m_guiEntity->SetRotation(Quaternion::Rotation(m_currentRotation, Vec3(0.0f, 1.0f, 0.0f)));
	}

	void SetVisible(bool t_visible) override {
		m_guiEntity->SetVisible(t_visible);
		m_label->SetVisible(t_visible);
		this->GUI::SetVisible(t_visible);
	}

protected:
	void AddedToGame() override {
		GameObjects::Add(m_guiEntity);
		GameObjects::Add((std::shared_ptr<Clickable>)m_label);

		m_guiEntity->AddedToGame();
		m_label->AddedToGame();
	}

	void AddedToParent(std::shared_ptr<Clickable> t_parent) override {
		m_guiEntity->SetPosition(this->GetPosition() + Vec3(0.0f, 0.2f, 10.0f));
		m_label->SetPosition(this->GetPosition() + Vec3(0.0f, 0.6f, 2.0f));
	}

	void ChangedPosition() override {
		m_guiEntity->SetPosition(this->GetPosition() + Vec3(0.0f, 0.2f, 10.0f));
		m_label->SetPosition(this->GetPosition() + Vec3(0.0f, 0.6f, 2.0f));
	}

private:
	static void SpawnObject(Clickable* t_clickable) {
		EntityDisplay* entityDisplay = (EntityDisplay*)t_clickable;
		if (entityDisplay->m_spawnedEntity == nullptr) {
			Vec3 position = ActiveCamera::GetActiveCamera()->GetPosition() + MousePickerPerspective::GetCurrentRay() * 50.0f;
			entityDisplay->m_spawnedEntity = std::make_shared<Entity>(Entity(TexturedModel(entityDisplay->m_guiEntity->GetRawModel(), entityDisplay->m_guiEntity->GetTexture(), 0), entityDisplay->m_guiEntity->GetNormalMap(), position, Quaternion(1.0f, 0.0f, 0.0f, 0.0f)/*TODO: ActiveCamera::GetActiveCamera()->GetViewDirection()*/, Vec3(1.0f), Vec3(0.0f), true, false, "bounce"));
			GameObjects::Add(entityDisplay->m_spawnedEntity);
		} else {
			Vec3 position = ActiveCamera::GetActiveCamera()->GetPosition() + MousePickerPerspective::GetCurrentRay() * 50.0f;
			entityDisplay->m_spawnedEntity->SetPosition(position);
		}
	}
	static void HoldObject(Clickable* t_clickable) {
		EntityDisplay* entityDisplay = (EntityDisplay*)t_clickable;
		float scale = entityDisplay->m_scale;
		if (Input::GetKey(GLFW_KEY_P)) scale += 0.1f;
		else if (Input::GetKey(GLFW_KEY_O)) scale -= 0.1f;
		entityDisplay->m_scale = scale;

		if (entityDisplay->m_spawnedEntity == nullptr) return;
		Vec3 viewDirection = ActiveCamera::GetActiveCamera()->GetViewDirection();
		viewDirection.y = 0.0f;
		Vec3 position = ActiveCamera::GetActiveCamera()->GetPosition() + MousePickerPerspective::GetCurrentRay() * 50.0f;
		entityDisplay->m_spawnedEntity->SetPosition(position);
		entityDisplay->m_spawnedEntity->SetScale(scale);
		entityDisplay->m_spawnedEntity->SetRotation(Quaternion(1.0f, 0.0f, 0.0f, 0.0f));//TODO: viewDirection);
	}
	static void ReleaseObject(Clickable* t_clickable) {
		EntityDisplay* entityDisplay = (EntityDisplay*)t_clickable;
		if (entityDisplay->m_spawnedEntity == nullptr) return;
		entityDisplay->m_spawnedEntity = nullptr;
	}
};
