#pragma once
#include "TextRenderer.h"

class TextMaster {

private:
	TextShader m_textShader = TextShader();
	TextRenderer m_textRenderer;

public:
	TextMaster(const Mat4& t_orthographicMatrix) : m_textRenderer(m_textShader) {
		m_textShader.BindShader();
		m_textShader.LoadProjectionMatrix(t_orthographicMatrix);
		m_textShader.UnbindShader();
	}
	
	void Render(std::map<GLuint, std::vector<std::shared_ptr<Text>>>& t_texts) {
		m_textShader.BindShader();
		m_textRenderer.Render(t_texts);
		m_textShader.UnbindShader();
	}	
	void CleanUp() {
		m_textShader.CleanUp();
	}
};