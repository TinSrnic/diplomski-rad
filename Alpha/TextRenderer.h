#pragma once
#include "TextShader.h"
#include "Text.h"

/*Class in charge of rendering text*/
class TextRenderer {

private:
	TextShader& m_textShader;
	Vec3 m_currentTextPointer = Vec3(0.0f, 0.0f, 0.0f);
	float view = 0.0f;

public:
	TextRenderer(TextShader& t_fontShader) : m_textShader(t_fontShader) { }

	//TODO: future me, this won't be able to change textures so find a way around it, I don't want to deal with it
	void Render(std::map<GLuint, std::vector<std::shared_ptr<Text>>>& t_texts) {
		if (t_texts.size() == 0) return;
		Prepare();
		for each(std::pair<GLuint, std::vector<std::shared_ptr<Text>>> pair in t_texts) {
			PrepareFont(pair.first);
			for each(std::shared_ptr<Text> text in pair.second) {
				PrepareText(text);
				for each(EditableCharacter c in text->GetCharacters()) {
					PrepareCharacter(c);
					float size = c.GetSize();
					Vec2 charOffset = c.GetAtlasOffset();
					Vec3 offset = Vec3(m_currentTextPointer.x + charOffset.x * size, m_currentTextPointer.y - charOffset.y * size, 0.0f);
					m_textShader.LoadModelMatrix(Mat4::Translation(offset) * Mat4::Scale(Vec3(size)));
					m_currentTextPointer.x += c.GetXAdvance() * size;
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				}
			}
		}
		Unbind();
	}

private:
	void Prepare() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void PrepareFont(const GLuint t_fontTexture) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_fontTexture);
	}
	void PrepareText(const std::shared_ptr<Text>& t_text) {
		m_currentTextPointer = t_text->GetPosition();
	}
	void PrepareCharacter(const EditableCharacter& t_character) {
		glBindVertexArray(t_character.GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		int useOutline = t_character.GetUseOutline() ? 1 : 0;
		m_textShader.LoadTextColor(t_character.GetColor());
		m_textShader.LoadUseOutline(t_character.GetUseOutline());
		if (useOutline) {
			m_textShader.LoadTextOutlineColor(t_character.GetOutlineColor());
			m_textShader.LoadOffset(t_character.GetOutlineWidth());
		}
	}
	void Unbind() {
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glBindVertexArray(0);
	}
};