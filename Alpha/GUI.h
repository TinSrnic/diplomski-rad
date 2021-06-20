#pragma once
#include "TexturedModel.h"
#include "Math.h"

/*Class used to represent an GUI element identified by its name.*/
class GUI : public TexturedModel {

private:
	std::string m_name;

	Vec3 m_position;
	Quaternion m_rotation;
	Vec3 m_scale;
	Mat4 m_modelMatrix;
	Vec4 m_color;
	bool m_useCustomColor;

	bool m_affectedByLight;

	GLuint m_normalMap;

	std::string m_sound;

	bool m_visible = true;
	int m_textureMultiple = 1;

public:
	GUI(TexturedModel& t_texturedModel, GLuint t_normalMap, const std::string& t_name, const Vec3& t_position, const Quaternion& t_rotation, const Vec3& t_scale, const bool t_affectedByLight, const std::string& t_sound)
		: m_normalMap(t_normalMap), m_name(t_name), TexturedModel(t_texturedModel), m_position(t_position), m_rotation(t_rotation), m_scale(t_scale), m_affectedByLight(t_affectedByLight), m_sound(t_sound) {
		CalculateModelMatrix();
		m_color = Vec4(0);
		m_useCustomColor = false;
	}
	GUI(TexturedModel& t_texturedModel, GLuint t_normalMap, const std::string& t_name, const Vec3& t_position, const Quaternion& t_rotation, const Vec3& t_scale, const bool t_affectedByLight, const std::string& t_sound, Vec4 t_color)
		: m_normalMap(t_normalMap), m_name(t_name), TexturedModel(t_texturedModel), m_position(t_position), m_rotation(t_rotation), m_scale(t_scale), m_affectedByLight(t_affectedByLight), m_sound(t_sound) {
		CalculateModelMatrix();
		m_color = t_color;
		m_useCustomColor = true;
	}
	
	std::string GetName() const { return m_name; }
	Vec3 GetPosition() const { return m_position; }
	Quaternion GetRotation() const { return m_rotation; }
	Vec3 GetScale() const { return m_scale; }
	Mat4 GetModelMatrix() const { return m_modelMatrix; }
	bool GetAffectedByLight() const { return m_affectedByLight; }

	GLuint GetNormalMap() const { return m_normalMap; }

	int GetTextureMultiple() const { return m_textureMultiple; }

	Vec4 getColor() const { return m_color; }
	bool getUseCustomColor() const { return m_useCustomColor; }

	std::string GetSound() const { return m_sound; }

	bool GetVisible() const { return m_visible; }

	void SetName(const std::string& t_name) { m_name = t_name; }
	void SetPosition(const Vec3& t_position) { 
		m_position = t_position; 
		CalculateModelMatrix();
	}
	void SetRotation(const Quaternion& t_rotation) { 
		m_rotation = Quaternion::Normalize(t_rotation); 
		CalculateModelMatrix();
	}
	void SetScale(const Vec3& t_scale) {
		m_scale = t_scale;
		CalculateModelMatrix();
	}
	void SetTextureMultiple(const int t_textureMultiple) {
		m_textureMultiple = t_textureMultiple;
	}

	void SetAffectedByLight(const bool t_affectedByLight) { m_affectedByLight = t_affectedByLight; }

	void SetTexture(const Texture texture) {
		m_texture = texture;
	}

	void SetSound(const std::string& t_sound) { m_sound = t_sound; }

	virtual void SetVisible(bool t_visible) { m_visible = t_visible; }

	virtual void Update() {

	}

	virtual void UpdateSound() {

	}

	virtual void AddedToGame() {

	}

	virtual void ChangedPosition() { }

private:
	void CalculateModelMatrix() { m_modelMatrix = Mat4::Translation(m_position) * m_rotation.ConvertToMat4() * Mat4::Scale(m_scale); }

};