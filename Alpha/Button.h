#pragma once
#include "BoundedGUI.h"
#include <iostream>
#include <functional>

/*Class used to store the data for button rendering and action on click*/
class Button : public BoundedGUI {

private:
	std::function<void()> m_clickCall;
	bool m_primed = false;

public:
	Button(BoundedGUI t_boundedGUI, std::function<void()> t_clickCall) 
		: BoundedGUI(t_boundedGUI), m_clickCall(t_clickCall) { }

	void SetPrimed() { m_primed = true; }
	void ResetPrimed() { m_primed = false; }
	bool IsPrimed() { return m_primed; }

	void ActionOnClick() {
		m_clickCall();
	}
};