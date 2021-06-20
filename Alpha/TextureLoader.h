#pragma once
#include <glew.h>
#include <glfw3.h>
#include <map>
#include <vector>
#include <string>
#include "Texture.h"

/*
Class used to manage all loaded textures can be used for massive speed up(maybe, at least during load time)
*/
class TextureLoader {

private:
	static std::map<std::string, GLuint> m_textures;
	static std::map<std::vector<std::string>, GLuint> m_cubeTextures;

public:
	static GLuint LoadTexture(const std::string& filepath);
	static Texture LoadTexture(const std::string& filepath, int numOfRows, float reflectivity, float shineDampening);
	static GLuint LoadCubeTexture(const std::vector<std::string>& t_filepaths);
	static void LoadCursorTexture(const std::string& t_filename, GLFWwindow* t_window);
	static void GetTextureDimensions(const std::string& t_filename, unsigned int* t_height, unsigned int* t_width = nullptr);
	static void GetTexturePixels(const std::string& t_filename, unsigned char*& t_pixels);

private:
	static GLuint LoadImageData(const std::string& t_filepath);
	static GLuint LoadImageData(const std::string& t_filepath, int& t_width, int& t_height);
	static GLuint LoadCubeMap(const std::vector<std::string>& t_filenames);
};