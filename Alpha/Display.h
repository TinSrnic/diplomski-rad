#pragma once
#include "Vec2.h"

class Display {

	friend class Window;

public:
	static unsigned int m_windowWidth;
	static unsigned int m_windowHeight;

	static unsigned int GetWindowWidth() { return m_windowWidth; }
	static unsigned int GetWindowHeight() { return m_windowHeight; }
	static float GetAspectRatio() { return (float)m_windowWidth / m_windowHeight; }
};