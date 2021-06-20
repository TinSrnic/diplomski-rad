#pragma once
#include "ComputeShader.h"
#include <vector>
#include <iostream>
#include "FileReader.h"
#include "Logger.h"

ComputeShader::ComputeShader() {
	m_computeShaderID = 0;
	m_programID = 0;
}

ComputeShader::ComputeShader(const std::string& computeShaderPath) {
	m_computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	std::string computeShaderCode = ReadFile(computeShaderPath);

	char const* computeShaderSource = computeShaderCode.c_str();
	glShaderSource(m_computeShaderID, 1, &computeShaderSource, NULL);
	glCompileShader(m_computeShaderID);

	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(m_computeShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(m_computeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(m_computeShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		Logger::Error(&VertexShaderErrorMessage[0]);
	}

	m_programID = glCreateProgram();
	glAttachShader(m_programID, m_computeShaderID);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(m_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		Logger::Error(&ProgramErrorMessage[0]);
	}
}

void ComputeShader::SetUniform1i(GLint location, int value) const { glUniform1i(location, value); }
void ComputeShader::SetUniform1f(GLint location, float value) const { glUniform1f(location, value); }
void ComputeShader::SetUniformVec2(GLint location, const Vec2& vec) const { glUniform2f(location, vec.x, vec.y); }
void ComputeShader::SetUniformVec3(GLint location, const Vec3& vec) const { glUniform3f(location, vec.x, vec.y, vec.z); }
void ComputeShader::SetUniformVec4(GLint location, const Vec4& vec) const { glUniform4f(location, vec.x, vec.y, vec.z, vec.w); }
void ComputeShader::SetUniformMat4(GLint location, const Mat4& mat) const { glUniformMatrix4fv(location, 1, GL_FALSE, mat.m_elements); }

void ComputeShader::BindShader() { glUseProgram(m_programID); }
void ComputeShader::UnbindShader() { glUseProgram(0); }

void ComputeShader::CleanUp() {
	UnbindShader();
	glDetachShader(m_programID, m_computeShaderID);

	glDeleteShader(m_computeShaderID);

	glDeleteProgram(m_programID);
}