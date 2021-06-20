#pragma once
#include <glew.h>
#include "Math.h"

class ComputeShader {

public:
	GLuint m_computeShaderID;
	GLuint m_programID;

public:
	ComputeShader();
	ComputeShader(const std::string& computeShaderPath);

	void SetUniform1i(GLint location, int value) const;
	void SetUniform1f(GLint location, float value) const;
	void SetUniformVec2(GLint location, const Vec2& vec) const;
	void SetUniformVec3(GLint location, const Vec3& vec) const;
	void SetUniformVec4(GLint location, const Vec4& vec) const;
	void SetUniformMat4(GLint location, const Mat4& mat) const;

public:
	void BindShader();
	void UnbindShader();

	void CleanUp();
};