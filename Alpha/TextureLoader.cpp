#include "TextureLoader.h"
#include <FreeImage.h>
#include "WorldConstants.h"

std::map<std::string, GLuint> TextureLoader::m_textures = std::map<std::string, GLuint>();
std::map<std::vector<std::string>, GLuint> TextureLoader::m_cubeTextures = std::map<std::vector<std::string>, GLuint>();

GLuint TextureLoader::LoadTexture(const std::string& filepath) {
	try {
		return m_textures.at(filepath);
	} catch (std::out_of_range&) {
		return m_textures.insert(std::pair<std::string, GLuint>(filepath, LoadImageData(filepath))).first->second;
	}
}
 Texture TextureLoader::LoadTexture(const std::string& filepath, int numOfRows, float reflectivity, float shineDampening) {
	int width = 0;
	int height = 0;
	GLuint textureID = 0;

	try {
		textureID = m_textures.at(filepath);
		LoadImageData(filepath, width, height);
	} catch (std::out_of_range&) {
		textureID = LoadImageData(filepath, width, height);
		m_textures.insert(std::pair<std::string, GLuint>(filepath, textureID));
	}

	return Texture(textureID, numOfRows, reflectivity, shineDampening, width, height);
}
GLuint TextureLoader::LoadCubeTexture(const std::vector<std::string>& t_filepaths) {
	try {
		return m_cubeTextures.at(t_filepaths);
	} catch (std::out_of_range&) {
		return m_cubeTextures.insert(std::pair<std::vector<std::string>, GLuint>(t_filepaths, LoadCubeMap(t_filepaths))).first->second;
	}
}
void TextureLoader::LoadCursorTexture(const std::string& t_filename, GLFWwindow* t_window) {
	std::string filepath = CURSOR_PATH + t_filename;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);

	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (fif == FIF_UNKNOWN) return;

	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filepath.c_str());
	if (!dib) return;

	FreeImage_FlipVertical(dib);

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	GLFWimage image;
	image.width = height;
	image.height = width;
	image.pixels = bits;

	GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);

	glfwSetCursor(t_window, cursor);
}
void TextureLoader::GetTextureDimensions(const std::string& t_filename, unsigned int* t_height, unsigned int* t_width) {
	std::string filepath = TEXTURE_PATH + t_filename;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	unsigned int width(0), height(0);

	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (fif == FIF_UNKNOWN) return;

	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filepath.c_str());
	if (!dib) return;

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	if ((width == 0) || (height == 0)) return;

	if (t_height != nullptr) *t_height = height;
	if (t_width != nullptr) *t_width = width;
}
void TextureLoader::GetTexturePixels(const std::string& t_filename, unsigned char*& t_pixels) {
	std::string filepath = TEXTURE_PATH + t_filename;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	unsigned int width(0), height(0);

	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (fif == FIF_UNKNOWN) return;

	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filepath.c_str());
	if (!dib) return;

	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	if ((width == 0) || (height == 0)) return;

	FreeImage_FlipVertical(dib);

	t_pixels = FreeImage_GetBits(dib);
}

GLuint TextureLoader::LoadImageData(const std::string& t_filepath) {
	std::string filepath = TEXTURE_PATH + t_filepath;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint gl_texID;

	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (fif == FIF_UNKNOWN) return false;

	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filepath.c_str());
	if (!dib) return false;

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	if ((bits == 0) || (width == 0) || (height == 0)) return false;

	glGenTextures(1, &gl_texID);

	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glGenerateMipmap(GL_TEXTURE_2D);

	return gl_texID;
}
GLuint TextureLoader::LoadImageData(const std::string& t_filepath, int& t_width, int& t_height) {
	std::string filepath = TEXTURE_PATH + t_filepath;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP* dib(0);
	BYTE* bits(0);
	unsigned int width(0), height(0);
	GLuint gl_texID;

	fif = FreeImage_GetFileType(filepath.c_str(), 0);
	if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filepath.c_str());
	if (fif == FIF_UNKNOWN) return false;

	if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, filepath.c_str());
	if (!dib) return false;

	bits = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	if ((bits == 0) || (width == 0) || (height == 0)) return false;

	t_width = width;
	t_height = height;
	glGenTextures(1, &gl_texID);

	glBindTexture(GL_TEXTURE_2D, gl_texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glGenerateMipmap(GL_TEXTURE_2D);

	return gl_texID;
}

GLuint TextureLoader::LoadCubeMap(const std::vector<std::string>& t_filenames) {
	GLuint gl_texID;
	glGenTextures(1, &gl_texID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, gl_texID);

	for (int x = 0; x < 6; x++) {
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP* dib(0);
		BYTE* bits(0);
		unsigned int width(0), height(0);

		std::string currentPath = SKYBOX_PATH + t_filenames.at(x);
		fif = FreeImage_GetFileType(currentPath.c_str(), 0);
		if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(currentPath.c_str());
		if (fif == FIF_UNKNOWN) return false;

		if (FreeImage_FIFSupportsReading(fif)) dib = FreeImage_Load(fif, currentPath.c_str());
		if (!dib) return false;

		bits = FreeImage_GetBits(dib);
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		if ((bits == 0) || (width == 0) || (height == 0)) return false;
		unsigned int size = width * height * 3;
		BYTE* flipBits = (BYTE*)malloc(size);
		for (unsigned int y = size - 1; y > 0; y--) flipBits[size - y - 1] = bits[y];

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + x, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, flipBits);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return gl_texID;
}