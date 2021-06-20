#pragma once
#include "Character.h"

/*Class used to store all the characters of a font*/
class Font {

private:
	std::string m_fontName;
	std::map<unsigned int, Character> m_characters;
	GLuint m_textureID;
	unsigned int m_imageHeight;

public:
	Font(const std::string& t_fontName, const std::map<unsigned int, Character>& t_characters, const GLuint t_textureID, const unsigned int t_imageHeight) {
		m_fontName = t_fontName;
		m_characters = t_characters;
		m_textureID = t_textureID;
		m_imageHeight = t_imageHeight;
	}

	GLuint GetTextureID() const { return m_textureID; }
	Character& GetCharacter(const char c) {
		auto character = m_characters.find(c);
		if (character == m_characters.end()) return m_characters.at('e');
		else return character->second;
	}
};