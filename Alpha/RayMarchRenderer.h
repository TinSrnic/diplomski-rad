#pragma once
#include <map>
#include "RayMarchShader.h"
#include "GUI.h"
#include "FluidParticle.h"

/*Class used to render GUI objects*/
class RayMarchRenderer {

private:
	RayMarchShader& m_RayMarchShader;
	GLuint m_fluidParticleData;
	GLuint m_particleTexture;

public:
	RayMarchRenderer(RayMarchShader& t_shader, const Mat4& t_orthographicMatrix, const Mat4& t_perspectiveMatrix, GLuint particleTexture, Vec3 boxMin, Vec3 boxMax) : m_RayMarchShader(t_shader) {
		m_RayMarchShader.BindShader();
		m_RayMarchShader.LoadOrthographicMatrix(t_orthographicMatrix);
		m_RayMarchShader.LoadPerspectiveMatrix(t_perspectiveMatrix);
		m_RayMarchShader.LoadBoxMin(boxMin);
		m_RayMarchShader.LoadBoxMax(boxMax);
		m_RayMarchShader.UnbindShader();

		glGenTextures(1, &m_fluidParticleData);
		m_particleTexture = particleTexture;
	}

	void Render(GUI& t_gui, float* fluidParticles, int fluidParticleCount, float blendCoeficient, float moveFactor, float viewportWidth, float viewportHeight, Vec3& lightPos, Mat4& viewMatrix, Vec3& cameraPos, Vec3& cameraViewDirection) {
			m_RayMarchShader.BindShader();
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_RayMarchShader.LoadViewportWidth(viewportWidth);
			m_RayMarchShader.LoadViewportHeight(viewportHeight);
			m_RayMarchShader.LoadLightPos(lightPos);
			m_RayMarchShader.LoadViewMatrix(viewMatrix);
			m_RayMarchShader.LoadCameraPos(cameraPos);
			m_RayMarchShader.LoadCameraViewDirection(cameraViewDirection);
			m_RayMarchShader.LoadFluidParticleCount(fluidParticleCount);
			m_RayMarchShader.LoadBlendCoefiecient(blendCoeficient);
			m_RayMarchShader.LoadMoveFactor(moveFactor);

			PrepareGUI(t_gui);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_1D, m_fluidParticleData);
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB32F, fluidParticleCount, 0, GL_RGB, GL_FLOAT, fluidParticles);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_particleTexture);

			glDrawElements(GL_TRIANGLES, t_gui.GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			UnbindTexturedModel();
			glDisable(GL_BLEND);
			m_RayMarchShader.UnbindShader();
	}

private:
	void PrepareGUI(GUI& t_gui) {
		glBindVertexArray(t_gui.GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		m_RayMarchShader.LoadModelMatrix(t_gui.GetModelMatrix());
	}
	void UnbindTexturedModel() {
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};