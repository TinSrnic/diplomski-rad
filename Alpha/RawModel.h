#pragma once
#include <glew.h>
#include "Vec3.h"
#include "WorldConstants.h"

/* Class used to construct and store opengl vaos and ibos which hold the data for a model */
class RawModel {

protected:
	GLuint m_VaoID;
	GLuint m_IboID;
	GLuint m_vertexCount;
	GLuint m_indicesCount;
	Vec3 m_dimensions;

public:
	RawModel(GLfloat* t_vertices, GLuint t_vertexCount, GLfloat* t_UVs, GLfloat* t_normals, GLuint* t_indices, GLuint t_indicesCount, const Vec3& t_dimensions)
		: m_vertexCount(t_vertexCount), m_indicesCount(t_indicesCount) {
		m_dimensions = t_dimensions;
		GLuint vertBufferID, uvBufferID, normBufferID;

		glGenVertexArrays(1, &m_VaoID);
		glBindVertexArray(m_VaoID);

		glGenBuffers(1, &vertBufferID);
		glGenBuffers(1, &uvBufferID);
		glGenBuffers(1, &normBufferID);
		glGenBuffers(1, &m_IboID);

		StoreDataInAttribArray(vertBufferID, t_vertices, 3 * t_vertexCount * sizeof(GLfloat), VERTEX_LOCATION, 3);
		StoreDataInAttribArray(uvBufferID, t_UVs, 2 * t_vertexCount * sizeof(GLfloat), UV_LOCATION, 2);
		StoreDataInAttribArray(normBufferID, t_normals, 3 * t_vertexCount * sizeof(GLfloat), NORMAL_LOCATION, 3);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indicesCount * sizeof(GLuint), t_indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
	RawModel(const RawModel& t_other) {
		m_VaoID = t_other.m_VaoID;
		m_IboID = t_other.m_IboID;
		m_vertexCount = t_other.m_vertexCount;
		m_indicesCount = t_other.m_indicesCount;
		m_dimensions = t_other.m_dimensions;
	}

	GLuint GetVaoID() const { return m_VaoID; }
	GLuint GetIboID() const { return m_IboID; }
	GLuint GetVertexCount() const { return m_vertexCount; }
	GLuint GetIndicesCount() const { return m_indicesCount; }

	Vec3 GetDimensions() const { return m_dimensions; }

private:
	//Helper function used to shorten code in constructor
	void StoreDataInAttribArray(GLuint t_bufferID, GLfloat* t_buffer, GLuint t_count, GLuint t_attribLocation, GLint t_size) {
		glBindBuffer(GL_ARRAY_BUFFER, t_bufferID);
		glBufferData(GL_ARRAY_BUFFER, t_count, t_buffer, GL_STATIC_DRAW);
		glVertexAttribPointer(t_attribLocation, t_size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
};