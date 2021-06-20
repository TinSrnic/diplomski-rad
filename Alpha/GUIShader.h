#pragma once
#include "ShaderProgram.h"

/*Class used to manage the GUI shader*/
class GUIShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_modelMatrix;
	GLint m_affectedByLight;
	GLint m_lightDirection;
	GLint m_viewDirection;
	GLint m_color;
	GLint m_useCustomColor;
	GLint m_textureMultiple;

public:
	GUIShader() : ShaderProgram(GUI_VERT_PATH, GUI_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");

		m_affectedByLight = glGetUniformLocation(m_programID, "affectedByLight");
		m_lightDirection = glGetUniformLocation(m_programID, "lightDirection");
		m_viewDirection = glGetUniformLocation(m_programID, "viewDirection");
		m_color = glGetUniformLocation(m_programID, "color");
		m_useCustomColor = glGetUniformLocation(m_programID, "useCustomColor");
		m_textureMultiple = glGetUniformLocation(m_programID, "textureMultiple");
	}

	void LoadProjectionMatrix(const Mat4& mat) { SetUniformMat4(m_projectionMatrix, mat); }
	void LoadModelMatrix(const Mat4& mat) { SetUniformMat4(m_modelMatrix, mat); }
	void LoadColor(const Vec4& t_color) { SetUniformVec4(m_color, t_color); }
	void LoadUseCustomColor(const int useCustomColor) { SetUniform1i(m_useCustomColor, useCustomColor); }
	void LoadTextureMultiple(const int textureMultiple) { SetUniform1i(m_textureMultiple, textureMultiple); }

	void LoadAffectedByLight(const float t_affected) { SetUniform1f(m_affectedByLight, t_affected); }
	void LoadLightDirection(const Vec3& t_direction) { SetUniformVec3(m_lightDirection, t_direction); }
	void LoadViewDirection(const Vec3& t_direction) { SetUniformVec3(m_viewDirection, t_direction); }
};