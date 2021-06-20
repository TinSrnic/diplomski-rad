#pragma once
#include "Font.h"
#include "EditableCharacter.h"

/*Class used to represent a string with variable font, color and size*/
class Text {

private:
	Font& m_font;
	std::string m_text;
	std::vector<EditableCharacter> m_characters;
	Vec3 m_position;
	
public:
	Text(Font& t_font, const std::string& t_text, const Vec3& position) : m_font(t_font) {
		m_position = position;
		m_text = t_text;
		for each(char c in m_text) {
			m_characters.push_back(EditableCharacter(t_font.GetCharacter(c), Vec3(1.0f), 3.0f));
		}
	}

	Font& GetFont() { return m_font; }
	std::string GetText() const { return m_text; }
	std::vector<EditableCharacter>& GetCharacters() { return m_characters; }
	Vec3 GetPosition() const { return m_position; }

	void SetFont(Font& t_font) {
		std::vector<EditableCharacter> newCharacters = std::vector<EditableCharacter>();
		for each (EditableCharacter character in m_characters) {
			newCharacters.push_back(EditableCharacter(t_font.GetCharacter(character.GetAsciiCode()), character.GetColor(), character.GetSize()));
		}
		m_font = t_font;
	}
	void SetSize(unsigned int t_startPos, unsigned int t_endPos, float t_size) {
		if (t_startPos > t_endPos) return;
		std::vector<EditableCharacter>::iterator it;
		for (it = m_characters.begin() + t_startPos; it != m_characters.begin() + t_endPos; it++) {
			it->SetSize(t_size);
		}
	}
	void SetColor(unsigned int t_startPos, unsigned int t_endPos, const Vec3& t_color) {
		if (t_startPos > t_endPos) return;
		std::vector<EditableCharacter>::iterator it;
		for (it = m_characters.begin() + t_startPos; it != m_characters.begin() + t_endPos; it++) {
			it->SetColor(t_color);
		}
	}
	void SetOutlineColor(unsigned int t_startPos, unsigned int t_endPos, const Vec3& t_outlineColor) {
		if (t_startPos > t_endPos) return;
		std::vector<EditableCharacter>::iterator it;
		for (it = m_characters.begin() + t_startPos; it != m_characters.begin() + t_endPos; it++) {
			it->SetOutlineColor(t_outlineColor);
		}
	}

	void SetText(std::string& t_text, Font& t_font) {
		m_text.clear();
		m_text = t_text;

		m_characters.clear();
		m_font = t_font;
		for each(char c in m_text) {
			m_characters.push_back(EditableCharacter(m_font.GetCharacter(c), Vec3(1.0f), 3.0f));
		}
	}
	void SetPosition(const Vec3& t_position) { m_position = t_position; }

	void RemoveCharacters(unsigned int t_startPos, unsigned int t_endPos) {
		m_text.erase(t_startPos, t_endPos);
		m_characters.erase(m_characters.begin() + t_startPos, m_characters.begin() + t_endPos);
	}
};