#pragma once
#include "Input.h"
#include "Display.h"
#include "WorldConstants.h"

class MousePickerOrthographic {

private:
	static Vec2 m_currentRay;

public:
	static void Update() {
		//To openGL screen coords(normalized device space)
		int windowWidth = Display::GetWindowWidth();
		int windowHeight = Display::GetWindowHeight();

		Vec2 mousePos = Input::GetMousePosition();

		//mouse coords in [-1, 1]
		mousePos.x = (mousePos.x - (windowWidth / 2.0f)) / (windowWidth / 2.0f);
		mousePos.y = (mousePos.y - (windowHeight / 2.0f)) / (windowHeight / 2.0f);

		//To homogenus clip space
		Vec4 clipCoords = Vec4(mousePos, -1, 1);

		//To eye space (inverse projection matrix)
		Vec4 eyeCoords = Mat4::Invert(World::GetOrthographicMatrix()) * clipCoords;
		eyeCoords = Vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
		
		//for GUI
		m_currentRay = Vec2(eyeCoords.x, ORTHOGRAPHIC_TOP - eyeCoords.y);
	}

	static Vec2 GetCurrentRay() { return m_currentRay; }
};