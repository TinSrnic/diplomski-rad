#pragma once
#include <glew.h>
#include <vector>
#include <iostream>
#include "WorldConstants.h"
#include "Math.h"
#include "FileReader.h"
#include "Logger.h"

class ShaderProgram {

protected:
	GLuint m_vertID;
	GLuint m_fragID;
	GLuint m_programID;

protected:
	ShaderProgram(const std::string& vertPath, const std::string& fragPath) {

		m_vertID = glCreateShader(GL_VERTEX_SHADER);
		m_fragID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::string VertexShaderCode = ReadFile(vertPath);
		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode = ReadFile(fragPath);

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		char const *VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(m_vertID, 1, &VertexSourcePointer, NULL);
		glCompileShader(m_vertID);

		// Check Vertex Shader
		glGetShaderiv(m_vertID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(m_vertID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(m_vertID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			Logger::Error(&VertexShaderErrorMessage[0]);
		}

		// Compile Fragment Shader
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(m_fragID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(m_fragID);

		// Check Fragment Shader
		glGetShaderiv(m_fragID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(m_fragID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(m_fragID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			Logger::Error(&FragmentShaderErrorMessage[0]);
		}

		// Link the program
		m_programID = glCreateProgram();
		glAttachShader(m_programID, m_vertID);
		glAttachShader(m_programID, m_fragID);
		glLinkProgram(m_programID);

		// Check the program
		glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(m_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			Logger::Error(&ProgramErrorMessage[0]);
		}
	}

	void SetUniform1i(GLint location, int value) const { glUniform1i(location, value); }
	void SetUniform1f(GLint location, float value) const { glUniform1f(location, value); }
	void SetUniformVec2(GLint location, const Vec2& vec) const { glUniform2f(location, vec.x, vec.y); }
	void SetUniformVec3(GLint location, const Vec3& vec) const { glUniform3f(location, vec.x, vec.y, vec.z); }
	void SetUniformVec4(GLint location, const Vec4& vec) const { glUniform4f(location, vec.x, vec.y, vec.z, vec.w); }
	void SetUniformMat4(GLint location, const Mat4& mat) const { glUniformMatrix4fv(location, 1, GL_FALSE, mat.m_elements); }

public:
	void BindShader() { glUseProgram(m_programID); }
	void UnbindShader() { glUseProgram(0); }

	void CleanUp() {
		UnbindShader();
		glDetachShader(m_programID, m_vertID);
		glDetachShader(m_programID, m_fragID);

		glDeleteShader(m_vertID);
		glDeleteShader(m_fragID);

		glDeleteProgram(m_programID);
	}
};