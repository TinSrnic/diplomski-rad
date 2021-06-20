#pragma once
#include "ShaderProgram.h"

/*Shader for water*/
class WaterShader : public ShaderProgram {

	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	GLint m_reflectionTexture;
	GLint m_refractionTexture;

	GLint m_dudvMapTexture;
	GLint m_normalMapTexture;
	GLint m_depthMapTexture;

	GLint m_waveStrength;
	GLint m_moveFactor;

	GLint m_cameraPosition;
	GLint m_waterRefraction;

	GLint m_lightPosition;
	GLint m_lightColor;
	GLint m_shineDamper;
	GLint m_reflectivity;

	GLint m_near;
	GLint m_far;
	GLint m_maxWaterDepth;

	GLint m_drawDistance;

public:
	WaterShader() : ShaderProgram(WATER_VERT_PATH, WATER_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_reflectionTexture = glGetUniformLocation(m_programID, "reflectionTexture");
		m_refractionTexture = glGetUniformLocation(m_programID, "refractionTexture");
		m_dudvMapTexture = glGetUniformLocation(m_programID, "DuDvMapTexture");
		m_normalMapTexture = glGetUniformLocation(m_programID, "normalMapTexture");
		m_depthMapTexture = glGetUniformLocation(m_programID, "depthMapTexture");
		m_waveStrength = glGetUniformLocation(m_programID, "waveStrength");
		m_moveFactor = glGetUniformLocation(m_programID, "moveFactor");
		m_cameraPosition = glGetUniformLocation(m_programID, "cameraPosition");
		m_waterRefraction = glGetUniformLocation(m_programID, "waterRefraction");
		m_lightPosition = glGetUniformLocation(m_programID, "lightPosition");
		m_lightColor = glGetUniformLocation(m_programID, "lightColor");
		m_shineDamper = glGetUniformLocation(m_programID, "shineDamper");
		m_reflectivity = glGetUniformLocation(m_programID, "reflectivity");
		m_near = glGetUniformLocation(m_programID, "near");
		m_far = glGetUniformLocation(m_programID, "far");
		m_maxWaterDepth = glGetUniformLocation(m_programID, "maxWaterDepth");
		m_drawDistance = glGetUniformLocation(m_programID, "drawDistance");

		this->BindShader();
		SetUniform1i(m_reflectionTexture, 0);
		SetUniform1i(m_refractionTexture, 1);
		SetUniform1i(m_dudvMapTexture, 2);
		SetUniform1i(m_normalMapTexture, 3);
		SetUniform1i(m_depthMapTexture, 4);
		this->UnbindShader();
	}

	void LoadProjectionMatrix(const Mat4& mat) { SetUniformMat4(m_projectionMatrix, mat); }
	void LoadViewMatrix(const Mat4& mat) { SetUniformMat4(m_viewMatrix, mat); }
	void LoadModelMatrix(const Mat4& mat) { SetUniformMat4(m_modelMatrix, mat); }

	void LoadWaveStrength(float val) { SetUniform1f(m_waveStrength, val); }
	void LoadMoveFactor(float val) { SetUniform1f(m_moveFactor, val); }

	void LoadCameraPosition(const Vec3& vec) { SetUniformVec3(m_cameraPosition, vec); }
	void LoadWaterRefraction(float val) { SetUniform1f(m_waterRefraction, val); }

	void LoadLightPosition(const Vec3& vec) { SetUniformVec3(m_lightPosition, vec); }
	void LoadLightColor(const Vec3& vec) { SetUniformVec3(m_lightColor, vec); }
	void LoadShineDamper(float val) { SetUniform1f(m_shineDamper, val); }
	void LoadReflectivity(float val) { SetUniform1f(m_reflectivity, val); }

	void LoadNearFarPlane(float nearPlane, float farPlane) {
		SetUniform1f(m_near, nearPlane);
		SetUniform1f(m_far, farPlane);
	}
	void LoadMaxWaterDepth(float val) { SetUniform1f(m_maxWaterDepth, val); }

	void LoadDrawDistance(float t_drawDistance) { SetUniform1f(m_drawDistance, t_drawDistance); }
};