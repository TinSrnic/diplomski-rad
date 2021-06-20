#pragma once
#include "Animation.h"

class AnimatedModel {

protected:
	GLuint m_vaoID;
	GLuint m_textureID;
	int m_indicesCount;

	std::map<int, Animation*> m_animations = std::map<int, Animation*>();
	int m_currentAnimation = 0;

	Vec3 m_position;
	Mat4 m_modelMatrix;

public:
	AnimatedModel(GLfloat* t_positionArray, GLfloat* t_textureArray, 
		GLfloat* t_normalArray, GLint* t_jointArray, GLfloat* t_weightArray, GLuint* t_indicesArray, 
		unsigned int t_vertexCount, unsigned int t_indicesCount, Animation* t_animation, GLuint t_textureID) {
		m_animations.insert(std::pair<int, Animation*>(0, t_animation));
		m_position = Vec3(0.0f);
		m_modelMatrix = Mat4::Translation(m_position);
		m_indicesCount = t_indicesCount;
		m_textureID = t_textureID;

		GLuint positionBuffer, textureBuffer, normalBuffer, jointBuffer, weightBuffer, indexBuffer;

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &positionBuffer);
		glGenBuffers(1, &textureBuffer);
		glGenBuffers(1, &normalBuffer);
		glGenBuffers(1, &jointBuffer);
		glGenBuffers(1, &weightBuffer);
		glGenBuffers(1, &indexBuffer);

		StoreDataInAttribArray(positionBuffer, t_positionArray, t_vertexCount * 3 * sizeof(GLfloat), 0, 3);
		StoreDataInAttribArray(textureBuffer, t_textureArray, t_vertexCount * 2 * sizeof(GLfloat), 1, 2);
		StoreDataInAttribArray(normalBuffer, t_normalArray, t_vertexCount * 3 * sizeof(GLfloat), 2, 3);
		
		glBindBuffer(GL_ARRAY_BUFFER, jointBuffer);
		glBufferData(GL_ARRAY_BUFFER, t_vertexCount * 3 * sizeof(GLint), t_jointArray, GL_STATIC_DRAW);
		glVertexAttribIPointer(3, 3, GL_INT, 0, (void*)0);

		StoreDataInAttribArray(weightBuffer, t_weightArray, t_vertexCount * 3 * sizeof(GLfloat), 4, 3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indicesCount * sizeof(GLuint), t_indicesArray, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	GLuint GetVaoID() { return m_vaoID; }
	GLuint GetTextureID() { return m_textureID; }
	int GetIndicesCount() { return m_indicesCount; }
	Vec3 GetPosition() { return m_position; }
	Mat4 GetModelMatrix() { return Mat4::Translation(m_position); }

	Animation* GetCurrentAnimation() {
		return m_animations.at(m_currentAnimation);
	}

	void SetPosition(const Vec3& t_position) { m_position = t_position; }

	void Update(float t_frameTime) {
		Animation* currentAnimation = m_animations.at(m_currentAnimation);
		currentAnimation->Update(t_frameTime);
	}

private:
	void StoreDataInAttribArray(GLuint bufferID, GLfloat* buffer, GLuint count, GLuint attribLocation, GLint size) {
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);
		glBufferData(GL_ARRAY_BUFFER, count, buffer, GL_STATIC_DRAW);
		glVertexAttribPointer(attribLocation, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
};