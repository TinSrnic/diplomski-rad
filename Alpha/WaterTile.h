#pragma once
#include "Math.h"
#include "TextureLoader.h"

/*Class used to represent a single quad of water*/
class WaterTile {

private:
	//the two maps need to match each other otherwise it looks wierd. This is simply done by creating them that way. 
	//(just check how the pictures look like)
	GLuint m_dudvMapTextureID; //this makes it seem like the water has waves on its surface
	GLuint m_normalMapTextureID; //normal maps are used to simulate dynamic lighting as the water moves
	Vec3 m_viewDirection; //basically rotation
	Vec3 m_position;
	Vec3 m_scale;
	Mat4 m_modelMatrix;

public:
	WaterTile(const std::string& t_dudvMapPath, const std::string& t_normalMapPath, const Vec3& t_position, const Vec3& t_scale, const Vec3& t_viewDirection = Vec3(0.0f, 0.0f, -1.0f)) {
		m_dudvMapTextureID = TextureLoader::LoadTexture(t_dudvMapPath);
		m_normalMapTextureID = TextureLoader::LoadTexture(t_normalMapPath);
		m_viewDirection = Vec3::Normalize(t_viewDirection);
		m_position = t_position;
		m_scale = t_scale;
		CalculateModelMatrix();
	}

	GLuint GetDuDvMapTextureID() const { return m_dudvMapTextureID; }
	GLuint GetNormalMapTextureID() const { return m_normalMapTextureID; }
	Vec3 GetViewDirection() const { return m_viewDirection; }
	Vec3 GetPosition() const { return m_position; }
	Vec3 GetScale() const { return m_scale; }
	Mat4 GetModelMatrix() const { return m_modelMatrix; }

	void SetViewDirection(const Vec3& t_viewDirection) {
		m_viewDirection = Vec3::Normalize(t_viewDirection);
		CalculateModelMatrix();
	}
	void SetPosition(const Vec3& t_position) {
		m_position = t_position;
		CalculateModelMatrix();
	}
	void SetScale(const Vec3& t_scale) {
		m_scale = t_scale;
		CalculateModelMatrix();
	}

private:
	void CalculateModelMatrix() { 
		m_modelMatrix = Mat4::Translation(m_position) *  Mat4::RotationFromDirection(m_viewDirection) * Mat4::Scale(m_scale);
	}
};