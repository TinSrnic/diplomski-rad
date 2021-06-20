#pragma once
#include "Clickable.h"

/*Class used for grouping GUIs together*/
class Group : public Clickable {

protected:
	std::vector<std::pair<std::shared_ptr<Clickable>, Vec3>> m_controls = std::vector<std::pair<std::shared_ptr<Clickable>, Vec3>>();

public:
	Group(const BoundedGUI& t_boundedGUI) : Clickable(t_boundedGUI, std::function<void(Clickable*)>()) { }

	void AddControl(const std::shared_ptr<Clickable>& t_gui, const Vec3& t_position) {
		t_gui->SetPosition(this->GUI::GetPosition() + t_position);
		m_controls.push_back(std::pair<std::shared_ptr<Clickable>, Vec3>(t_gui, t_position));
		t_gui->AddedToParent(std::make_shared<Clickable>(*this));
	}
	std::vector<std::pair<std::shared_ptr<Clickable>, Vec3>>& GetControls() { return m_controls; }

	void RemoveControl(const std::string& t_name) {
		std::remove_if(m_controls.begin(), m_controls.end(), [&](const std::pair<std::shared_ptr<Clickable>, Vec3>& gui) { return t_name == gui.first->GetName(); });
	}

	void SetPosition(const Vec3& t_position) {
		this->Clickable::SetPosition(t_position);
		for each (std::pair<std::shared_ptr<Clickable>, Vec3> controlPositionPair in m_controls) {
			std::shared_ptr<Clickable>& control = controlPositionPair.first;
			control->SetPosition(controlPositionPair.second + t_position);
		}
		
	}

	void AddedToGame() override {
		for each (std::pair<std::shared_ptr<Clickable>, Vec3> controlPositionPair in m_controls) {
			GameObjects::Add(controlPositionPair.first);
		}
	}

	void ChangedPosition() override {
		for each (std::pair<std::shared_ptr<Clickable>, Vec3> controlPositionPair in m_controls) {
			controlPositionPair.first->SetPosition(this->GetPosition() + controlPositionPair.second);
		}
	}

	void SetVisible(bool t_visible) override {
		this->GUI::SetVisible(t_visible);
		for each (std::pair<std::shared_ptr<Clickable>, Vec3> controlPositionPair in m_controls) {
			controlPositionPair.first->SetVisible(t_visible);
		}
	}
};