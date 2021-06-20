#pragma once
#include "Camera.h"
#include "WaterFBO.h"
#include "WaterShader.h"
#include "WaterTile.h"

/*Class for rendering water tiles*/
class WaterRenderer {

private:
	WaterShader& m_waterShader;
	GLuint m_vaoID; //vao for quad of water
	float m_moveFactor = 0; //for wave movment

public:
	WaterRenderer(WaterShader& t_waterShader) : m_waterShader(t_waterShader) {
		const GLfloat vertices[12] = { -1, 0, 1, 1, 0, 1, -1, 0, -1, 1, 0, -1 };

		GLuint vertBufferID;

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &vertBufferID);

		glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}

	void Render(const std::list<std::shared_ptr<WaterTile>>& t_waterTiles, const Camera& t_camera, const WaterFBO& t_waterFBO, const Light& t_sun)  {
		Prepare(t_camera, t_waterFBO, t_sun);
		for each(std::shared_ptr<WaterTile> tile in t_waterTiles) {
			PrepareWaterTile(tile);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}
		UnbindWaterTile();
		m_moveFactor += 0.001f;
		if (m_moveFactor >= 1.0f) m_moveFactor = 0.0f;
	}

private:
	void Prepare(const Camera& t_camera, const WaterFBO& t_waterFBO, const Light& t_sun) {
		glBindVertexArray(m_vaoID);
		glEnableVertexAttribArray(0);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_waterFBO.GetReflectionTexture());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, t_waterFBO.GetRefractionTexture());

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, t_waterFBO.GetRefractionDepthTexture());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_waterShader.LoadViewMatrix(t_camera.GetViewMatrix());
		m_waterShader.LoadCameraPosition(t_camera.GetPosition());
		m_waterShader.LoadLightPosition(t_sun.GetPosition());
		m_waterShader.LoadLightColor(t_sun.GetColor());
		m_waterShader.LoadMoveFactor(m_moveFactor);
	}
	void PrepareWaterTile(const std::shared_ptr<WaterTile>& t_tile) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, t_tile->GetDuDvMapTextureID());

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, t_tile->GetNormalMapTextureID());

		m_waterShader.LoadModelMatrix(t_tile->GetModelMatrix());

	}
	void UnbindWaterTile() {
		glDisable(GL_BLEND);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

};