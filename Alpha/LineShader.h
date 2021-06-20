#pragma once
#include "ShaderProgram.h"

class LineShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

public:
	LineShader() : ShaderProgram(LINE_VERT_PATH, LINE_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
	}

	void LoadProjectionMatrix(const Mat4& t_projectionMatrix) { SetUniformMat4(m_projectionMatrix, t_projectionMatrix); }
	void LoadViewMatrix(const Mat4& t_viewMatrix) { SetUniformMat4(m_viewMatrix, t_viewMatrix); }
	void LoadModelMatrix(const Mat4& t_modelMatrix) { SetUniformMat4(m_modelMatrix, t_modelMatrix); }
};