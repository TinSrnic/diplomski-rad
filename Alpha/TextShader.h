#pragma once
#include "ShaderProgram.h"

/*Shader in charge of rendering text*/
class TextShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_modelMatrix;
	GLint m_textColor;
	GLint m_useOutline; //0 = don't use, 1 = use
	GLint m_textOutlineColor;
	GLint m_offset; //offset for outline TODO: test what this does exactly

public:
	TextShader() : ShaderProgram(FONT_VERT_PATH, FONT_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_textColor = glGetUniformLocation(m_programID, "textColor");
		m_useOutline = glGetUniformLocation(m_programID, "useOutline");
		m_textOutlineColor = glGetUniformLocation(m_programID, "textOutlineColor");
		m_offset = glGetUniformLocation(m_programID, "offset");
	}

	void LoadProjectionMatrix(const Mat4& t_mat) const { SetUniformMat4(m_projectionMatrix, t_mat); }
	void LoadModelMatrix(const Mat4& t_mat) const { SetUniformMat4(m_modelMatrix, t_mat); }
	void LoadTextColor(const Vec3& t_color) const { SetUniformVec3(m_textColor, t_color); }
	void LoadUseOutline(int t_use) const { SetUniform1i(m_useOutline, t_use); }
	void LoadTextOutlineColor(const Vec3& t_color) const { SetUniformVec3(m_textColor, t_color); }
	void LoadOffset(float t_offset) const { SetUniform1f(m_offset, t_offset); }
};