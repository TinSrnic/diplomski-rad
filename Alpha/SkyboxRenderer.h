#pragma once
#include "SkyboxShader.h"
#include "Skybox.h"

/*Class in charge of rendering skyboxes*/
class SkyboxRenderer {

private:
	SkyboxShader& m_GUIShader;

public:
	SkyboxRenderer(SkyboxShader& t_skyboxShader) : m_GUIShader(t_skyboxShader) {}

	void Render(const std::shared_ptr<Skybox>& t_skybox, float t_currentTime) {
		PrepareSkybox(t_skybox, t_currentTime);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		UnbindSkybox();
	}

private:
	void PrepareSkybox(const std::shared_ptr<Skybox>& t_skybox, float t_currentTime) {
		glBindVertexArray(t_skybox->GetVaoID());
		glEnableVertexAttribArray(0);

		BindTextures(t_skybox->GetCubeMapSunriseID(), t_skybox->GetCubeMapDayID(), t_skybox->GetCubeMapSunsetID(), t_skybox->GetCubeMapNightID(), t_currentTime);
	}
	void BindTextures(GLint t_sunriseTexture, GLint t_dayTexture, GLint t_sunsetTexure, GLint t_nightTexture, float t_currentTime) {
		int texture1;
		int texture2;
		float blendFactor;

		if (t_currentTime >= 0 && t_currentTime < 80) {
			texture1 = t_sunriseTexture;
			texture2 = t_dayTexture;
			blendFactor = (t_currentTime - 0) / (80 - 0);
		} else if (t_currentTime >= 80 && t_currentTime < 120) {
			texture1 = t_dayTexture;
			texture2 = t_sunsetTexure;
			blendFactor = (t_currentTime - 80) / (120 - 80);
		} else if (t_currentTime >= 120 && t_currentTime < 200) {
			texture1 = t_sunsetTexure;
			texture2 = t_nightTexture;
			blendFactor = (t_currentTime - 120) / (200 - 120);
		} else if (t_currentTime >= 200 && t_currentTime < 240) {
			texture1 = t_nightTexture;
			texture2 = t_sunriseTexture;
			blendFactor = (t_currentTime - 200) / (240 - 200);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
		m_GUIShader.LoadBlendFactor(blendFactor);
	}
	void UnbindSkybox() {
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};