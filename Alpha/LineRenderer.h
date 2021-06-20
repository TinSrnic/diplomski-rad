#pragma once
#include "LineShader.h"
#include "Line.h"
#include "WorldConstants.h"

class LineRenderer {

private:
	LineShader& m_lineShader;
	GLuint m_vao;
	GLuint m_vertexVbo;
	GLuint m_colorVbo;

public:
	LineRenderer(LineShader& t_lineShader) : m_lineShader(t_lineShader) {
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vertexVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_LINES * sizeof(Vec3) * 2, (void*)0, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &m_colorVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
		glBufferData(GL_ARRAY_BUFFER, MAX_LINES * sizeof(Vec4) * 2, (void*)0, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}

	void Render(const std::vector<std::shared_ptr<Line>>& t_lines) {
		glBindVertexArray(m_vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		GLfloat* vertArray = (GLfloat*)malloc(t_lines.size() * sizeof(Vec3) * 2);
		GLfloat* colorArray = (GLfloat*)malloc(t_lines.size() * sizeof(Vec4) * 2);

		int pointer = 0;
		int counter = 0;
		for each (std::shared_ptr<Line> line in t_lines) {
			if (++counter > MAX_LINES) break;
			Vec3 start = line->GetStartPoint();
			Vec3 end = line->GetEndPoint();

			vertArray[pointer++] = start.x;
			vertArray[pointer++] = start.y;
			vertArray[pointer++] = start.z;

			vertArray[pointer++] = end.x;
			vertArray[pointer++] = end.y;
			vertArray[pointer++] = end.z;
		}

		pointer = 0;
		counter = 0;
		for each (std::shared_ptr<Line> line in t_lines) {
			if (++counter > MAX_LINES) break;
			Vec4 start = line->GetStartColor();
			Vec4 end = line->GetEndColor();

			colorArray[pointer++] = start.x;
			colorArray[pointer++] = start.y;
			colorArray[pointer++] = start.z;
			colorArray[pointer++] = start.w;

			colorArray[pointer++] = end.x;
			colorArray[pointer++] = end.y;
			colorArray[pointer++] = end.z;
			colorArray[pointer++] = end.w;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, t_lines.size() * sizeof(Vec3) * 2, vertArray);

		glBindBuffer(GL_ARRAY_BUFFER, m_colorVbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, t_lines.size() * sizeof(Vec4) * 2, colorArray);

		glDrawArrays(GL_LINES, 0, t_lines.size() * 2);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

};