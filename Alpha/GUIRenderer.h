#pragma once
#include "GUIShader.h"
#include "GUI.h"

/*Class used to render GUI objects*/
class GUIRenderer {
	
private:
	GUIShader& m_GUIShader;

public:
	GUIRenderer(GUIShader& t_shader) : m_GUIShader(t_shader) { }

	void Render(std::map<GLuint, std::vector<std::shared_ptr<GUI>>>& t_GUIs) {
		for each(std::pair<GLuint, std::vector<std::shared_ptr<GUI>>> pair in t_GUIs) {
			PrepareTexturedModel(*(pair.second.begin()));
			for each(std::shared_ptr<GUI> gui in pair.second) {
				if (!gui->GetVisible()) continue;
				PrepareGUI(gui);
				glDrawElements(GL_TRIANGLES, gui->GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			}
			UnbindTexturedModel();
		}
	}

private:
	void PrepareTexturedModel(std::shared_ptr<TexturedModel> t_texturedModel) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_texturedModel->GetTexture().GetTextureID());
	}
	void PrepareGUI(std::shared_ptr<GUI> t_gui) { 
		glBindVertexArray(t_gui->GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		m_GUIShader.LoadAffectedByLight(t_gui->GetAffectedByLight() ? 1.0f : 0.0f);
		m_GUIShader.LoadModelMatrix(t_gui->GetModelMatrix()); 
		m_GUIShader.LoadTextureMultiple(t_gui->GetTextureMultiple());
		if (t_gui->getUseCustomColor()) {
			m_GUIShader.LoadUseCustomColor(1);
			m_GUIShader.LoadColor(t_gui->getColor());
		} else {
			m_GUIShader.LoadUseCustomColor(0);
		}
		
	}
	void UnbindTexturedModel() {
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};