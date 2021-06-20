#pragma once
#include "ShaderProgram.h"

/*Shader for shadows which are made using shadow maps*/
class ShadowShader : public ShaderProgram {
	
private:
	GLint m_modelMatrix;
	GLint m_viewProjectionMatrix;

public:
	ShadowShader() : ShaderProgram(SHADOW_VERT_PATH, SHADOW_FRAG_PATH) {
		m_viewProjectionMatrix = glGetUniformLocation(m_programID, "viewProjectionMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
	}

	void LoadModelMatrix(const Mat4& t_mat) { SetUniformMat4(m_modelMatrix, t_mat); }
	void LoadViewProjectionMatrix(const Mat4& t_mat) { SetUniformMat4(m_viewProjectionMatrix, t_mat); }
};
