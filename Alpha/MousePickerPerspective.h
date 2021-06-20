#pragma once
#include "Input.h"
#include "Display.h"
#include "WorldConstants.h"

class MousePickerPerspective {
	
private:
	static Vec3 m_currentRay;

public:
	static void Update(Mat4 viewMatrix) {
		//antongerdelan.net/opengl/raycasting.html
		//To openGL screen coords(normalized device space)
		int windowWidth = Display::GetWindowWidth();
		int windowHeight = Display::GetWindowHeight();

		Vec2 mousePos = Input::GetMousePosition(); //viewport space
		//normalised device space
		mousePos.x = 2 * mousePos.x / windowWidth - 1;
		mousePos.y = 2 * mousePos.y / windowHeight - 1;

		//clip space
		Vec4 clipCoords = Vec4(mousePos.x, -mousePos.y, -1, 1);

		//To eye space (inverse projection matrix)
		Vec4 eyeCoords = Mat4::Invert(World::GetPerspectiveMatrix()) * clipCoords;
		eyeCoords = Vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

		//For entity
		//To world space
		Vec4 worldCoords = Mat4::Invert(viewMatrix) * eyeCoords;
		m_currentRay = Vec3::Normalize(Vec3(worldCoords.x, worldCoords.y, worldCoords.z));
	}

	static Vec3 GetCurrentRay() { return m_currentRay; }
	static Vec3 GetNormalizedCurrentRay() { return Vec3::Normalize(m_currentRay); }
};
