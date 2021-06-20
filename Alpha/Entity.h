#pragma once
#include "TexturedModel.h"
#include "Math.h"
#include "Time.h"

/* Class for representing entities holding information about their position, rotation, size and velocity
as well as information about their model and texture in the form of inheritence.*/
class Entity : public TexturedModel {

protected:
	Vec3 m_position;
	Quaternion m_rotation;
	Vec3 m_scale;
	Vec3 m_velocity;
	Mat4 m_modelMatrix;

	bool m_isFloating;
	bool m_useFakeLighting;

	std::string m_sound;

	GLuint m_normalMap;

public:
	Entity(const TexturedModel& t_texturedModel, GLuint t_normalMap, const Vec3& t_position, const Quaternion& t_rotation, const Vec3& t_scale, const Vec3& t_velocity, bool t_isFloating, bool t_useFakeLighting, const std::string& t_sound)
		: TexturedModel(t_texturedModel), m_position(t_position), m_rotation(Quaternion::Normalize(t_rotation)), m_scale(t_scale), m_velocity(t_velocity), m_sound(t_sound) { 
		m_isFloating = t_isFloating;
		m_useFakeLighting = t_useFakeLighting;
		m_normalMap = t_normalMap;
		CalculateModelMatrix();
	}

	//Getters
	Vec3 GetPosition() const { return m_position; }
	Vec3 GetViewDirection() const { return Quaternion::RotatePoint(m_rotation, Vec3(0.0f, 0.0f, 1.0f)); }
	Vec3 GetScale() const { return m_scale; }
	Vec3 GetVelocity() const { return m_velocity; }
	Mat4 GetModelMatrix() const { return m_modelMatrix; }

	bool GetFloating() const { return m_isFloating; }
	bool GetFakeLighting() const { return m_useFakeLighting; }

	GLuint GetNormalMapID() { return m_normalMap; }

	std::string GetSound() const { return m_sound; }

	//Setters
	void SetPosition(const Vec3& t_position) { 
		m_position = t_position; 
		CalculateModelMatrix();
	}
	void SetRotation(const Quaternion& t_rotation) { 
		m_rotation = t_rotation;
		CalculateModelMatrix();
	}
	void SetScale(const Vec3& t_scale) {
		m_scale = t_scale;
		CalculateModelMatrix();
	}
	void SetVelocity(const Vec3& t_velocity) { 
		m_velocity = t_velocity; 
	}
	
	void SetModelMatrix(const Mat4& t_mat) { m_modelMatrix = t_mat; }

	void SetFloating(bool t_isFloating) { m_isFloating = t_isFloating; }
	void SetFakeLighting(bool t_useFakeLighting) { m_useFakeLighting = t_useFakeLighting; }

	void SetSound(const std::string& t_sound) { m_sound = t_sound; }

	//Functions
	virtual void Update() {
		if (m_isFloating) return;
		float deltaTime = Time::GetDeltaTime();
		m_velocity.y += GRAVITY * Time::GetDeltaTime();
		m_position += m_velocity * Time::GetDeltaTime();
		CalculateModelMatrix(); //TODO: make this not needed
	}
	virtual bool UpdateSound() {
		if (1 == 2) return true; //i want to play sound
		else return false;
	}

private:
	inline void CalculateModelMatrix() { m_modelMatrix = Mat4::Translation(m_position) * m_rotation.ConvertToMat4() * Mat4::Scale(m_scale); }
};