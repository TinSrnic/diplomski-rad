#pragma once
#include "ShaderProgram.h"

class MarchingCubeShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	GLint* m_lightPosition = new GLint[MAX_LIGHTS];
	GLint* m_lightColor = new GLint[MAX_LIGHTS];
	GLint* m_attenuation = new GLint[MAX_LIGHTS];
	GLint m_shineDampening;
	GLint m_reflectivity;
	GLint m_skyColor;

	GLint m_drawDistance;
	GLint m_terrainScale;

public:
	MarchingCubeShader() : ShaderProgram(MARCHINGCUBE_VERT_PATH, MARCHINGCUBE_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_shineDampening = glGetUniformLocation(m_programID, "shineDamper");
		m_reflectivity = glGetUniformLocation(m_programID, "reflectivity");
		m_skyColor = glGetUniformLocation(m_programID, "skyColor");
		m_drawDistance = glGetUniformLocation(m_programID, "drawDistance");
		m_terrainScale = glGetUniformLocation(m_programID, "terrainScale");

		for (int x = 0; x < MAX_LIGHTS; x++) {
			std::string currentLightPos = "lightPos[" + std::to_string(x) + "]";
			std::string currentLightColor = "lightColor[" + std::to_string(x) + "]";
			std::string currentAttenuation = "attenuation[" + std::to_string(x) + "]";
			m_lightPosition[x] = glGetUniformLocation(m_programID, currentLightPos.c_str());
			m_lightColor[x] = glGetUniformLocation(m_programID, currentLightColor.c_str());
			m_attenuation[x] = glGetUniformLocation(m_programID, currentAttenuation.c_str());
		}
	}

	void LoadProjectionMatrix(const Mat4& t_mat) { SetUniformMat4(m_projectionMatrix, t_mat); }
	void LoadViewMatrix(const Mat4& t_mat) { SetUniformMat4(m_viewMatrix, t_mat); }
	void LoadModelMatrix(const Mat4& t_mat) { SetUniformMat4(m_modelMatrix, t_mat); }

	void LoadLights(std::vector<Light>& t_lights) {
		for (unsigned int x = 0; x < t_lights.size() && x < MAX_LIGHTS; x++) {
			SetUniformVec3(m_lightPosition[x], t_lights.at(x).GetPosition());
			SetUniformVec3(m_lightColor[x], t_lights.at(x).GetColor());
			SetUniformVec3(m_attenuation[x], t_lights.at(x).GetAttenuation());
		}
	}
	void LoadShineVariables(float t_reflectivity, float t_shineDampening) {
		SetUniform1f(m_reflectivity, t_reflectivity);
		SetUniform1f(m_shineDampening, t_shineDampening);
	}

	void LoadSkyColor(const Vec3& t_skyColor) { SetUniformVec3(m_skyColor, t_skyColor); }

	void LoadDrawDistance(float t_drawDistance) { SetUniform1f(m_drawDistance, t_drawDistance); }

	void LoadTerrainScale(float t_terrainScale) { SetUniform1f(m_terrainScale, t_terrainScale); }
};