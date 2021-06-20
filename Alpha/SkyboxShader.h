#pragma once
#include "ShaderProgram.h"

/*Shader for skyboxes*/
class SkyboxShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_cubeMapFirst;
	GLint m_cubeMapSecond;
	GLint m_fogColor;
	GLint m_blendFactor;
	float m_rotation = 0;

public:
	SkyboxShader() : ShaderProgram(SKYBOX_VERT_PATH, SKYBOX_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_cubeMapFirst = glGetUniformLocation(m_programID, "cubeMapFirst");
		m_cubeMapSecond = glGetUniformLocation(m_programID, "cubeMapSecond");
		m_fogColor = glGetUniformLocation(m_programID, "fogColor");
		m_blendFactor = glGetUniformLocation(m_programID, "blendFactor");
	}

	void LoadProjectionMatrix(const Mat4& t_mat) { SetUniformMat4(m_projectionMatrix, t_mat); }
	void LoadViewMatrix(Mat4 t_mat, float t_skyboxRotation) { 
		t_mat[12] = 0;
		t_mat[13] = 0;
		t_mat[14] = 0;
		m_rotation += t_skyboxRotation;
		if (m_rotation >= 360) m_rotation -= 360;
		Mat4 rot = Mat4::Rotation(m_rotation, Vec3(0.0, 1.0, 0.0));
		SetUniformMat4(m_viewMatrix, t_mat * rot); 
	}
	void LoadFogColor(const Vec3& t_fogColor) { SetUniformVec3(m_fogColor, t_fogColor); }
	void LoadBlendFactor(float t_factor) { SetUniform1f(m_blendFactor, t_factor); }
	void ConnectTextures() {
		SetUniform1i(m_cubeMapFirst, 0);
		SetUniform1i(m_cubeMapSecond, 1);
	}
};