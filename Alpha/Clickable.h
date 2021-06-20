#pragma once
#include <functional>
#include "BoundedGUI.h"

class Clickable : public BoundedGUI {

private:
	bool m_mouseInObject = false;
	bool m_mouseHeld = false;
	std::function<void(Clickable*)> m_enterFunction;
	std::function<void(Clickable*)> m_clickFunction;
	std::function<void(Clickable*)> m_heldFunction;
	std::function<void(Clickable*)> m_releaseFunction;
	std::function<void(Clickable*)> m_leaveFunction;

public:
	Clickable(const BoundedGUI& t_boundedGUI, std::function<void(Clickable*)> t_clickFunction) 
		: BoundedGUI(t_boundedGUI), m_enterFunction(), m_heldFunction(), m_clickFunction(t_clickFunction), m_releaseFunction(), m_leaveFunction() { }

	void OnMouseEnter() {
		m_mouseInObject = true;
		if (m_enterFunction == nullptr) return;
		m_enterFunction(this);
	}
	void OnMouseClick() { 
		if (m_mouseHeld) return;
		m_mouseHeld = true;
		if (m_clickFunction == nullptr) return;
		m_clickFunction(this); 
	}
	void OnMouseHeld() {
		if (m_heldFunction == nullptr) return;
		m_heldFunction(this);
	}
	void OnMouseRelease() {
		m_mouseHeld = false;
		if (m_releaseFunction == nullptr) return;
		m_releaseFunction(this);
	}
	void OnMouseLeave() {
		m_mouseInObject = false;
		if (m_leaveFunction == nullptr) return;
		m_leaveFunction(this);
	}

	void SetMouseEnterFunction(const std::function<void(Clickable*)>& t_enterFunction) { m_enterFunction = t_enterFunction; }
	void SetMouseClickFunction(const std::function<void(Clickable*)>& t_clickFunction) { m_clickFunction = t_clickFunction; }
	void SetMouseHeldFunction(const std::function<void(Clickable*)>& t_heldFunction) { m_heldFunction = t_heldFunction; }
	void SetMouseReleaseFunction(const std::function<void(Clickable*)>& t_releaseFunction) { m_releaseFunction = t_releaseFunction; }
	void SetMouseLeaveFunction(const std::function<void(Clickable*)>& t_leaveFunction) { m_leaveFunction = t_leaveFunction; }

	bool GetMouseInObject() { return m_mouseInObject; }
	bool GetMouseHeld() { return m_mouseHeld; }

	virtual void AddedToParent(std::shared_ptr<Clickable> t_parent) { }
};