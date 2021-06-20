#pragma once
#include "ShaderProgram.h"

/*Class for link with particle shader*/
class ParticleShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_inverseRotViewMatrix;
	GLint m_numberOfRows;

	GLint m_drawDistance;
	
public:
	ParticleShader() : ShaderProgram(PARTICLE_VERT_PATH, PARTICLE_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_inverseRotViewMatrix = glGetUniformLocation(m_programID, "inverseRotViewMatrix");
		m_numberOfRows = glGetUniformLocation(m_programID, "numberOfRows");
		m_drawDistance = glGetUniformLocation(m_programID, "drawDistance");
	}

	void LoadProjectionMatrix(const Mat4& t_mat) { SetUniformMat4(m_projectionMatrix, t_mat); }
	void LoadViewMatrix(const Mat4& t_mat) { SetUniformMat4(m_viewMatrix, t_mat); }
	void LoadInverseRotationViewMatrix(const Mat4& t_mat) { SetUniformMat4(m_inverseRotViewMatrix, t_mat); }
	void LoadAtlasRows(unsigned int t_numberOfRows) { SetUniform1f(m_numberOfRows, (float)t_numberOfRows); }

	void LoadDrawDistance(float t_drawDistance) { SetUniform1f(m_drawDistance, t_drawDistance); }
};