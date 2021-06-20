#pragma once
#include "ShaderProgram.h"
#include "Light.h"

class TerrainShader : public ShaderProgram {

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

	GLint m_backgroundTexture;
	GLint m_rTexture;
	GLint m_gTexture;
	GLint m_bTexture;
	GLint m_blendMap;

	GLint m_clipPlane;

	GLint m_shadowMap;
	GLint m_toShadowSpace;
	GLint m_shadowDistance;
	GLint m_shadowTransition;

	GLint m_drawDistance;

public:
	TerrainShader() : ShaderProgram(TERRAIN_VERT_PATH, TERRAIN_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_shineDampening = glGetUniformLocation(m_programID, "shineDamper");
		m_reflectivity = glGetUniformLocation(m_programID, "reflectivity");
		m_skyColor = glGetUniformLocation(m_programID, "skyColor");
		m_backgroundTexture =  glGetUniformLocation(m_programID, "backgroundTexture");
		m_rTexture = glGetUniformLocation(m_programID, "rTexture");
		m_gTexture = glGetUniformLocation(m_programID, "gTexture");
		m_bTexture = glGetUniformLocation(m_programID, "bTexture");
		m_blendMap = glGetUniformLocation(m_programID, "blendMap");
		m_clipPlane = glGetUniformLocation(m_programID, "clipPlane");
		m_shadowMap = glGetUniformLocation(m_programID, "shadowMap");
		m_toShadowSpace = glGetUniformLocation(m_programID, "toShadowSpace");
		m_shadowDistance = glGetUniformLocation(m_programID, "shadowDistance");
		m_shadowTransition = glGetUniformLocation(m_programID, "shadowTransition");
		m_drawDistance = glGetUniformLocation(m_programID, "drawDistance");

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

	void LoadClipPlane(const Vec4& t_plane) { SetUniformVec4(m_clipPlane, t_plane); }

	void LoadToShadowSpace(const Mat4& t_mat) { SetUniformMat4(m_toShadowSpace, t_mat); }
	void LoadShadowDistance(float t_distance) { SetUniform1f(m_shadowDistance, t_distance); }
	void LoadShadowTransition(float t_transition) { SetUniform1f(m_shadowTransition, t_transition); }

	void LoadDrawDistance(float t_drawDistance) { SetUniform1f(m_drawDistance, t_drawDistance); }

	void ConnectTextures() {
		SetUniform1i(m_backgroundTexture, 0);
		SetUniform1i(m_rTexture, 1);
		SetUniform1i(m_gTexture, 2);
		SetUniform1i(m_bTexture, 3);
		SetUniform1i(m_blendMap, 4);
		SetUniform1i(m_shadowMap, 5);
	}
};