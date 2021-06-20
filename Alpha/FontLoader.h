#pragma once
#include <fstream>
#include <string>
#include <map>

class Font;
class Character;

class FontLoader {
	
private:
	static std::map<std::string, Font> m_fonts;

public:
	static Font& LoadFont(const std::string& t_filePath, const std::string& t_texturePath);

private:
	static Font ReadFont(const std::string& t_filePath, const std::string& t_texturePath);
};