#pragma once
#include <map>
#include <vector>
#include "Camera.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "SkyboxRenderer.h"
#include "WorldConstants.h"

/*Class in charge of rendering entities, terrain and skyboxes*/
class MasterRenderer {

private:
	TerrainShader m_terrainShader = TerrainShader();
	SkyboxShader m_GUIShader = SkyboxShader();
	EntityShader m_entityShader = EntityShader();

	TerrainRenderer m_terrainRenderer;
	SkyboxRenderer m_skyboxRenderer;
	EntityRenderer m_entityRenderer;

	float m_dimLightsFactor = 1.0f;

public:
	MasterRenderer(const Mat4& t_projectionMatrix)
		: m_entityRenderer(m_entityShader), m_terrainRenderer(m_terrainShader), m_skyboxRenderer(m_GUIShader) {
		Vec3 FOG_COLOR = Vec3(0.5f, 0.5f, 0.5f);

		m_entityShader.BindShader();
		m_entityShader.LoadProjectionMatrix(t_projectionMatrix);
		m_entityShader.LoadShadowDistance(SHADOW_DISTANCE);
		m_entityShader.LoadShadowTransition(SHADOW_TRANSITION);
		m_entityShader.LoadSkyColor(FOG_COLOR);
		m_entityShader.LoadDrawDistance(DRAW_DISTANCE);
		m_entityShader.ConnectTextures(); //TODO: think if this should even be an option, it can be done as soon as the shader is created
		m_entityShader.UnbindShader();

		m_terrainShader.BindShader();
		m_terrainShader.LoadProjectionMatrix(t_projectionMatrix);
		m_terrainShader.LoadShadowDistance(SHADOW_DISTANCE);
		m_terrainShader.LoadShadowTransition(SHADOW_TRANSITION);
		m_terrainShader.LoadSkyColor(FOG_COLOR);
		m_terrainShader.LoadDrawDistance(DRAW_DISTANCE);
		m_terrainShader.ConnectTextures();
		m_terrainShader.UnbindShader();

		m_GUIShader.BindShader();
		m_GUIShader.LoadProjectionMatrix(t_projectionMatrix);
		m_GUIShader.ConnectTextures();
		m_GUIShader.LoadFogColor(FOG_COLOR);
		m_GUIShader.UnbindShader();
	}

	void SetSkyColor(const Vec3& t_skyColor) {
		Vec3 FOG_COLOR = t_skyColor;

		m_entityShader.BindShader();
		m_entityShader.LoadSkyColor(FOG_COLOR);
		m_entityShader.UnbindShader();

		m_terrainShader.BindShader();
		m_terrainShader.LoadSkyColor(FOG_COLOR);
		m_terrainShader.UnbindShader();

		m_GUIShader.BindShader();
		m_GUIShader.LoadFogColor(FOG_COLOR);
		m_GUIShader.UnbindShader();
	}
	//used to clip stuff when rendering water reflection and refraction
	void LoadClipPlane(const Vec4& t_clipPlane) { 
		m_entityShader.BindShader();
		m_entityShader.LoadClipPlane(t_clipPlane);
		m_entityShader.UnbindShader();

		m_terrainShader.BindShader();
		m_terrainShader.LoadClipPlane(t_clipPlane);
		m_terrainShader.UnbindShader();
	}

	void Render(const std::list<std::shared_ptr<Terrain>>& t_terrains, const std::shared_ptr<Skybox>& t_skybox, 
		const std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities, const std::vector<Light>& t_lights, 
		const Camera& t_camera, const Mat4& t_toShadowSpace, GLuint t_shadowTexture, float t_time) {
		std::vector<Light> updatedLights;
		Mat4 viewMatrix = t_camera.GetViewMatrix();

		while (t_time >= 240) t_time -= 240;

		for each(Light light in t_lights) updatedLights.push_back(Light(light.GetPosition(), light.GetColor() * m_dimLightsFactor, light.GetAttenuation()));
		
		//Terrrain rendering
		m_terrainShader.BindShader();
		m_terrainShader.LoadLights(updatedLights);
		m_terrainShader.LoadViewMatrix(viewMatrix);
		m_terrainShader.LoadToShadowSpace(t_toShadowSpace);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, t_shadowTexture);
		m_terrainRenderer.Render(t_terrains);
		m_terrainShader.UnbindShader();

		//Skybox rendering
		m_GUIShader.BindShader();
		m_GUIShader.LoadViewMatrix(viewMatrix, t_skybox->GetRotationSpeed());
		m_skyboxRenderer.Render(t_skybox, (float)t_time);
		m_GUIShader.UnbindShader();

		//Entities rendering
		m_entityShader.BindShader();
		m_entityShader.LoadLights(updatedLights);
		m_entityShader.LoadViewMatrix(viewMatrix);
		m_entityShader.LoadToShadowSpace(t_toShadowSpace);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, t_shadowTexture);
 		m_entityRenderer.Render(t_entities);
		m_entityShader.UnbindShader();

		if (t_time >= 0 && t_time < 80) m_dimLightsFactor = 1.0f;
		else if (t_time >= 80 && t_time < 120) m_dimLightsFactor = 0.9f * (float)(120 - t_time) / 40.0f + 0.1f;
		else if (t_time >= 120 && t_time < 200) m_dimLightsFactor = 0.1f;
		else if (t_time >= 200 && t_time < 240) m_dimLightsFactor = 0.9f * (float)(t_time - 200) / 40.0f + 0.1f;
		m_dimLightsFactor = 1.0f;
	}
	void CleanUp() {
		m_terrainShader.CleanUp();
		m_GUIShader.CleanUp();
		m_entityShader.CleanUp();
	}
};