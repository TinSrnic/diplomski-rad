#pragma once
#include <glew.h>
#include <vector>
#include "Vec3.h"
#include "WorldConstants.h"

class MarchingCubeVAO {

protected:
	GLuint m_VaoID;
	GLuint m_vertexCount;
	Vec2i m_position;
	Mat4 m_modelMatrix;
	GLuint m_vertBufferID;
	GLuint m_normalBufferID;

public:
	MarchingCubeVAO() : m_VaoID(0), m_vertexCount(0) {}
	MarchingCubeVAO (Mat4 modelMatrix, Vec2i position, std::vector<Vec3>& triangles, std::vector<Vec3>& normals) {
		m_modelMatrix = modelMatrix;
		m_position = position;

		glGenVertexArrays(1, &m_VaoID);
		glBindVertexArray(m_VaoID);

		glGenBuffers(1, &m_vertBufferID);
		glGenBuffers(1, &m_normalBufferID);

		if (triangles.size() == 0) {
			m_vertexCount = 3;
			float* newTriangles = new float[3];
			float* newNormals = new float[3];
			for (int i = 0; i < 3; ++i) {
				newTriangles[i] = 0.0f;
				newNormals[i] = 0.0f;
			}
			StoreDataInAttribArray(m_vertBufferID, newTriangles, m_vertexCount * sizeof(float), VERTEX_LOCATION, 3);
			StoreDataInAttribArray(m_normalBufferID, newNormals, m_vertexCount * sizeof(float), NORMAL_LOCATION, 3);
		} else {
			m_vertexCount = triangles.size() * 3;
			StoreDataInAttribArray(m_vertBufferID, (float*)&triangles[0], m_vertexCount * sizeof(float), VERTEX_LOCATION, 3);
			StoreDataInAttribArray(m_normalBufferID, (float*)&normals[0], m_vertexCount * sizeof(float), NORMAL_LOCATION, 3);
		}
		glBindVertexArray(0);
	}

	GLuint GetVaoID() const { return m_VaoID; }
	GLuint GetVertexCount() const { return m_vertexCount; }
	Mat4 GetModelMatrix() const { return m_modelMatrix; }
	Vec2i GetPosition() const { return m_position; }

	void SetPosition(Vec2i t_position) { m_position = t_position; }

	void ReplaceData(Mat4 modelMatrix, Vec2i position, std::vector<Vec3>& triangles, std::vector<Vec3>& normals) {
		m_modelMatrix = modelMatrix;
		m_position = position;

		glBindVertexArray(m_VaoID);
		if (triangles.size() == 0) {
			m_vertexCount = 3;
			float* newTriangles = new float[3];
			float* newNormals = new float[3];
			for (int i = 0; i < 3; ++i) {
				newTriangles[i] = 0.0f;
				newNormals[i] = 0.0f;
			}
			glBindBuffer(GL_ARRAY_BUFFER, m_vertBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(float), newTriangles);

			glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(float), newNormals);
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, m_vertBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(float), (float*)&triangles[0]);

			glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertexCount * sizeof(float), (float*)&normals[0]);
		}
		glBindVertexArray(0);
	}

private:
	//Helper function used to shorten code in constructor
	void StoreDataInAttribArray(GLuint t_bufferID, GLfloat* t_buffer, GLuint t_count, GLuint t_attribLocation, GLint t_size) {
		glBindBuffer(GL_ARRAY_BUFFER, t_bufferID);
		glBufferData(GL_ARRAY_BUFFER, t_count, t_buffer, GL_STATIC_DRAW);
		glVertexAttribPointer(t_attribLocation, t_size, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
};