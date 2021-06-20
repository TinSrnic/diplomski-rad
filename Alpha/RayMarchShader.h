#pragma once
#include "ShaderProgram.h"

/*Class used to manage the GUI shader*/
class RayMarchShader : public ShaderProgram {

private:
	GLint m_orthographicMatrix;
	GLint m_perspectiveMatrix;
	GLint m_modelMatrix;
	GLint m_viewportWidth;
	GLint m_viewportHeight;
	GLint m_viewMatrix;
	GLint m_cameraPos;
	GLint m_cameraViewDirection;
	GLint m_lightPos;
	GLint m_fluidParticleCount;
	GLint m_blendCoeficient;
	GLint m_moveFactor;
	GLint m_boxMin;
	GLint m_boxMax;

public:
	RayMarchShader() : ShaderProgram(RAY_MARCH_VERT_PATH, RAY_MARCH_FRAG_PATH) {
		m_orthographicMatrix = glGetUniformLocation(m_programID, "orthographicMatrix");
		m_perspectiveMatrix = glGetUniformLocation(m_programID, "perspectiveMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_viewportWidth = glGetUniformLocation(m_programID, "viewportWidth");
		m_viewportHeight = glGetUniformLocation(m_programID, "viewportHeight");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_cameraPos = glGetUniformLocation(m_programID, "cameraPos");
		m_cameraViewDirection = glGetUniformLocation(m_programID, "cameraViewDirection");
		m_lightPos = glGetUniformLocation(m_programID, "lightPos");
		m_fluidParticleCount = glGetUniformLocation(m_programID, "fluidParticleCount");
		m_blendCoeficient = glGetUniformLocation(m_programID, "blendCoeficient");
		m_moveFactor = glGetUniformLocation(m_programID, "moveFactor");
		m_boxMin = glGetUniformLocation(m_programID, "boxMin");
		m_boxMax = glGetUniformLocation(m_programID, "boxMax");
	}

	void LoadOrthographicMatrix(const Mat4& mat) { SetUniformMat4(m_orthographicMatrix, mat); }
	void LoadPerspectiveMatrix(const Mat4& mat) { SetUniformMat4(m_perspectiveMatrix, mat); }

	void LoadModelMatrix(const Mat4& mat) { SetUniformMat4(m_modelMatrix, mat); }
	void LoadViewportWidth(const float width) { SetUniform1f(m_viewportWidth, width); }
	void LoadViewportHeight(const float height) { SetUniform1f(m_viewportHeight, height); }
	void LoadViewMatrix(const Mat4& mat) { SetUniformMat4(m_viewMatrix, mat); }
	void LoadCameraPos(const Vec3& position) { SetUniformVec3(m_cameraPos, position); }
	void LoadCameraViewDirection(const Vec3& direction) { SetUniformVec3(m_cameraViewDirection, direction); }
	void LoadLightPos(const Vec3& position) { SetUniformVec3(m_lightPos, position); }
	void LoadFluidParticleCount(const int count) { SetUniform1i(m_fluidParticleCount, count); }
	void LoadBlendCoefiecient(const float coeficient) { SetUniform1f(m_blendCoeficient, coeficient); }
	void LoadMoveFactor(const float moveFactor) { SetUniform1f(m_moveFactor, moveFactor); }

	void LoadBoxMin(const Vec3& boxMin) { SetUniformVec3(m_boxMin, boxMin); }
	void LoadBoxMax(const Vec3& boxMax) { SetUniformVec3(m_boxMax, boxMax); }
	
};