#pragma once
#include "GUIRenderer.h"

/*Class used to manage all GUI renderers*/
class GUIMaster {

private:
	GUIShader m_GUIShader = GUIShader();
	GUIRenderer m_GUIRenderer;

public:
	GUIMaster(const Mat4& t_projectionMatrix) : m_GUIRenderer(m_GUIShader) {
		m_GUIShader.BindShader();
		m_GUIShader.LoadProjectionMatrix(t_projectionMatrix);
		m_GUIShader.UnbindShader();
	}

	void Render(std::map<GLuint, std::vector<std::shared_ptr<GUI>>>& t_GUIs, const Light& t_light, const Vec3& t_viewDirection = Vec3(0.0f, 0.0f, -1.0f)) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_GUIShader.BindShader();
		m_GUIShader.LoadLightDirection(-t_light.GetPosition());
		m_GUIShader.LoadViewDirection(t_viewDirection);
		m_GUIRenderer.Render(t_GUIs);
		m_GUIShader.UnbindShader();
		glDisable(GL_BLEND);
	}

	void CleanUp() {
		m_GUIShader.CleanUp();
	}
};