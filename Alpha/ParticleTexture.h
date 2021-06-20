#pragma once
#include <glew.h>
#include <string>

/*Class used to represent the texture of a particle*/
class ParticleTexture {

private:
	GLuint m_textureID;
	unsigned int m_numberOfRows;
	bool m_useAddativeBlending;

public:
	ParticleTexture(GLuint t_textureID, unsigned int t_numberOfRows, bool t_useAddativeBlending) {
		m_textureID = t_textureID;
		m_numberOfRows = t_numberOfRows;
		m_useAddativeBlending = t_useAddativeBlending;
	}
	ParticleTexture(GLuint t_textureID, unsigned int t_numberOfRows) {
		m_textureID = t_textureID;
		m_numberOfRows = t_numberOfRows;
	}

	GLuint GetTextureID() const { return m_textureID; }
	unsigned int GetNumberOfRows() const { return m_numberOfRows; }
	bool GetUseAddative() const { return m_useAddativeBlending; }
};