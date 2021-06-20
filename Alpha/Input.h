#pragma once
#include "Display.h"
#include "Vec2.h"

class Input {

	friend class Window;

private:
	static bool m_keyPress[];
	static bool m_previousFrameKeyPress[];
	static bool m_keyHold[];
	static bool m_keyRelease[];

	static bool m_mousePress[];

	static Vec2 m_mousePosition;

	static Vec2 m_mouseScroll;

public:
	static bool GetKey(unsigned int t_keyCode) { return m_keyPress[t_keyCode] || m_keyHold[t_keyCode]; }
	static bool GetKeyDown(unsigned int t_keyCode) { return m_keyPress[t_keyCode]; }
	static bool GetKeyHold(unsigned int t_keyCode) { return m_keyHold[t_keyCode]; }
	static bool GetKeyPress(unsigned int t_keyCode) { return m_previousFrameKeyPress[t_keyCode]; }

	static bool GetMouseDown(unsigned int t_buttonCode) { return m_mousePress[t_buttonCode]; }

	static Vec2 GetMousePosition() { return m_mousePosition; }

	static Vec2 GetMouseScroll() { return m_mouseScroll; }
};