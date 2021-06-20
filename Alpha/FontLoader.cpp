#include "FontLoader.h"
#include "TextureLoader.h"
#include "Font.h"
#include "StringManipulation.h"
#include "WorldConstants.h"

std::map<std::string, Font> FontLoader::m_fonts = std::map<std::string, Font>();

Font& FontLoader::LoadFont(const std::string& t_filePath, const std::string& t_texturePath) {
	try {
		return m_fonts.at(t_filePath);
	} catch (std::out_of_range ex) {
		return m_fonts.insert(std::pair<std::string, Font>(t_filePath, ReadFont(t_filePath, t_texturePath))).first->second;
	}
}

Font FontLoader::ReadFont(const std::string& t_filePath, const std::string& t_texturePath) {
	std::string fullFilePath = FONT_PATH + t_filePath;
	std::map<unsigned int, Character> characters;

	unsigned int imageHeight = 0;
	GLuint textureID = 0;

	TextureLoader::GetTextureDimensions(t_texturePath, &imageHeight);
	textureID = TextureLoader::LoadTexture(t_texturePath);

	std::ifstream infile(fullFilePath);
	std::string line;

	while (std::getline(infile, line)) {
		line = StringManipulation::RemoveConsecutiveChar(line, ' ');
		std::vector<std::string> data = StringManipulation::SplitString(line, ' ');
		if (data[0] == "kerning") break;
		else if (data[0] == "char") {
			unsigned int asciiCode = stoi(data[1].substr(3, data[1].length()));
			unsigned int xPos = stoi(data[2].substr(2, data[2].length()));
			unsigned int yPos = stoi(data[3].substr(2, data[3].length()));
			unsigned int width = stoi(data[4].substr(6, data[4].length()));
			unsigned int height = stoi(data[5].substr(7, data[5].length()));
			int xOffset = stoi(data[6].substr(8, data[6].length()));
			int yOffset = stoi(data[7].substr(8, data[7].length()));
			unsigned int xAdvance = stoi(data[8].substr(9, data[8].length()));
			characters.emplace(asciiCode, Character(imageHeight, xPos, yPos, width, height, xOffset, yOffset, xAdvance, asciiCode));
		} else continue;
	}

	return Font(t_filePath, characters, textureID, imageHeight);
}