#pragma once
#include <glew.h>
#include "Math.h"

class Font;
/*Class used to represent a single letter or more generally a character.*/
class Character {

private:
	Vec2 m_offset; //how far of the cursor the top left of the quad is
	float m_xAdvance; //how far the cursor has to move after the letter render
	GLuint m_vaoID; //vao for the character
	unsigned int m_asciiCode;

public:
	Character(unsigned int t_imageHeight, unsigned int t_xPos, unsigned int t_yPos, unsigned int t_width, unsigned int t_height, int t_xOffset, int t_yOffset, int t_xAdvance, unsigned int t_asciiCode) {
		m_asciiCode = t_asciiCode;

		m_offset = Vec2((float)t_xOffset / t_imageHeight, (float)(t_yOffset + t_height) / t_imageHeight);
		m_xAdvance = (float)t_xAdvance / t_imageHeight;

		float GLwidth = (float)t_width / t_imageHeight;
		float GLheight = (float)t_height / t_imageHeight;
		GLfloat vertices[8] = {
			0.0f, 0.0f,
			GLwidth, 0.0f,
			0.0f, GLheight,
			GLwidth, GLheight
		};

		float GLxPos = (float)t_xPos / t_imageHeight;
		float GLyPos = 1 - (float)t_yPos / t_imageHeight;

		GLfloat textures[8] = {
			GLxPos, GLyPos - GLheight,
			GLxPos + GLwidth, GLyPos - GLheight,
			GLxPos, GLyPos,
			GLxPos + GLwidth, GLyPos
		};

		GLuint vertexBuffer, textureBuffer;

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &textureBuffer);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, textures, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}

	GLuint GetVaoID() const { return m_vaoID; }
	Vec2 GetAtlasOffset() const { return m_offset; }
	float GetXAdvance() const { return m_xAdvance; }
	unsigned int GetAsciiCode() const { return m_asciiCode; }
};