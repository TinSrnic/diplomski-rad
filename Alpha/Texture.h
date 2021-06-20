#pragma once
#include <glew.h>

/* Class used to store data about an opengl texture as well as some extra info such as reflectivity and shine dampening used in the shader.
It also holds the number of rows a given texture has.*/
class Texture {

protected:
	GLuint m_textureID;
	unsigned int m_numberOfRows;

	float m_reflectivity;
	float m_shineDampening;

	float m_width;
	float m_height;

public:
	Texture() {
		m_textureID = 0;
		m_numberOfRows = 0;
		m_reflectivity = 0.0f;
		m_shineDampening = 0.0f;
		m_width = 0.0f;
		m_height = 0.0f;
	}

	Texture(GLuint t_texID, unsigned int t_numberOfRows, float t_reflectivity, float t_shineDampening)
		: m_textureID(t_texID), m_numberOfRows(t_numberOfRows), m_reflectivity(t_reflectivity), m_shineDampening(t_shineDampening), m_width(0.0f), m_height(0.0f) { }

	Texture(GLuint t_texID, unsigned int t_numberOfRows, float t_reflectivity, float t_shineDampening, float width, float height)
		: m_textureID(t_texID), m_numberOfRows(t_numberOfRows), m_reflectivity(t_reflectivity), m_shineDampening(t_shineDampening), m_width(width), m_height(height) { }

	GLuint GetTextureID() const { return m_textureID; }
	unsigned int GetNumberOfRows() const { return m_numberOfRows; }

	float GetReflectivity() const { return m_reflectivity; }
	float GetShineDampening() const { return m_shineDampening; }

	float GetWidth() const { return m_width; }
	float GetHeight() const { return m_height; }

	void SetReflectivity(float t_reflectivity) { m_reflectivity = t_reflectivity; }
	void SetShineDampening(float t_shineDampening) { m_shineDampening = t_shineDampening; }
};