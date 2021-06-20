#pragma once
#include <string>
#include "ShaderProgram.h"
#include "Light.h"
#include "Camera.h"
#define MAX_LIGHTS 4
#define MAX_JOINTS 50

class AnimationShader : public ShaderProgram {

private:
	GLint m_projectionMatrix;
	GLint m_viewMatrix;
	GLint m_modelMatrix;

	GLint* m_lightPosition = new GLint[MAX_LIGHTS];
	GLint* m_lightColor = new GLint[MAX_LIGHTS];
	GLint* m_attenuation = new GLint[MAX_LIGHTS];
	
	GLint m_modelTexture;
	GLint m_shineDamper;
	GLint m_reflectivity;
	GLint m_fakeLighting;

	GLint m_skyColor;

	GLint m_numberOfRows;
	GLint m_offset;
	GLint m_clipPlane;

	GLint m_density;
	GLint m_gradient;

	GLint m_shadowMap;
	GLint m_toShadowSpace;
	GLint m_shadowDistance;
	GLint m_shadowTransition;

	GLint* m_jointMatrix = new GLint[MAX_JOINTS];

	GLint m_drawDistance;

public:
	AnimationShader() : ShaderProgram(ANIMATION_VERT_PATH, ANIMATION_FRAG_PATH) {
		m_projectionMatrix = glGetUniformLocation(m_programID, "projectionMatrix");
		m_viewMatrix = glGetUniformLocation(m_programID, "viewMatrix");
		m_modelMatrix = glGetUniformLocation(m_programID, "modelMatrix");
		m_modelTexture = glGetUniformLocation(m_programID, "modelTexture");
		m_shineDamper = glGetUniformLocation(m_programID, "shineDamper");
		m_reflectivity = glGetUniformLocation(m_programID, "reflectivity");
		m_fakeLighting = glGetUniformLocation(m_programID, "fakeLighting");
		m_skyColor = glGetUniformLocation(m_programID, "skyColor");
		m_numberOfRows = glGetUniformLocation(m_programID, "numberOfRows");
		m_offset = glGetUniformLocation(m_programID, "offset");
		m_clipPlane = glGetUniformLocation(m_programID, "clipPlane");
		m_density = glGetUniformLocation(m_programID, "density");
		m_gradient = glGetUniformLocation(m_programID, "gradient");
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

		for (int x = 0; x < MAX_JOINTS; ++x) {
			std::string currentJointMatrix = "jointMatrix[" + std::to_string(x) + "]";
			m_jointMatrix[x] = glGetUniformLocation(m_programID, currentJointMatrix.c_str());
		}
	}

	void LoadLights(const std::vector<Light>& lights) {
		for (int x = 0; x < MAX_LIGHTS; x++) {
			SetUniformVec3(m_lightPosition[x], lights.at(x).GetPosition());
			SetUniformVec3(m_lightColor[x], lights.at(x).GetColor());
			SetUniformVec3(m_attenuation[x], lights.at(x).GetAttenuation());
		}
	}
	void LoadProjectionMatrix(Mat4 mat) { SetUniformMat4(m_projectionMatrix, mat); }
	void LoadViewMatrix(Mat4 mat) { SetUniformMat4(m_viewMatrix, mat); }
	void LoadModelMatrix(Mat4 mat) { SetUniformMat4(m_modelMatrix, mat); }
	void LoadShineVariables(float reflectivity, float shineDampening) {
		SetUniform1f(m_reflectivity, reflectivity);
		SetUniform1f(m_shineDamper, shineDampening);
	}
	void LoadFakeLighting(float fakeLighting) { SetUniform1f(m_fakeLighting, fakeLighting); }
	void LoadSkyColor(Vec3 skyColor) { SetUniformVec3(m_skyColor, skyColor); }
	void LoadNumberOfRows(unsigned int numberOfRows) { SetUniform1f(m_numberOfRows, (float)numberOfRows); }
	void LoadAtlasOffset(Vec2 offset) { SetUniformVec2(m_offset, offset); }
	void LoadClipPlane(Vec4 plane) { SetUniformVec4(m_clipPlane, plane); }

	void LoadDensity(float density) { SetUniform1f(m_density, density); }
	void LoadGradient(float gradient) { SetUniform1f(m_density, gradient); }

	void LoadToShadowSpace(const Mat4& t_mat) { SetUniformMat4(m_toShadowSpace, t_mat); }
	void LoadShadowDistance(float t_distance) { SetUniform1f(m_shadowDistance, t_distance); }
	void LoadShadowTransition(float t_transition) { SetUniform1f(m_shadowTransition, t_transition); }

	void LoadDrawDistance(float t_drawDistance) { SetUniform1f(m_drawDistance, t_drawDistance); }

	void ConnectTextures() {
		SetUniform1i(m_modelTexture, 0);
		SetUniform1i(m_shadowMap, 1);
	}

	void LoadJointMatrix(int t_index, const Mat4& t_jointMatrix) { 
		SetUniformMat4(m_jointMatrix[t_index], t_jointMatrix); }
};