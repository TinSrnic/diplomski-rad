#pragma once
#include "AnimationRenderer.h"

class AnimationMaster {

private:
	AnimationShader m_animationShader = AnimationShader();
	AnimationRenderer m_animationRenderer;

public:
	AnimationMaster(const Mat4& t_projectionMatrix) : m_animationRenderer(m_animationShader) {
		m_animationShader.BindShader();
		m_animationShader.LoadProjectionMatrix(t_projectionMatrix);
		m_animationShader.LoadShadowDistance(SHADOW_DISTANCE);
		m_animationShader.LoadShadowTransition(SHADOW_TRANSITION);
		m_animationShader.LoadSkyColor(Vec3(0.6f, 0.5f, 0.5f));
		m_animationShader.LoadDrawDistance(DRAW_DISTANCE);
		m_animationShader.ConnectTextures(); //TODO: think if this should even be an option, it can be done as soon as the shader is created
		m_animationShader.UnbindShader();
	}

	void Render(std::map<GLuint, std::vector<std::shared_ptr<AnimatedModel>>>& t_models, const Camera& t_camera, const std::vector<Light>& t_lights, float t_frameTime) {
		for each (std::pair<GLuint, std::vector<std::shared_ptr<AnimatedModel>>> pair in t_models) {
			for each (std::shared_ptr<AnimatedModel> model in pair.second) {
				model->Update(t_frameTime); //TODO: remove this
			}
		}

		m_animationShader.BindShader();
		m_animationShader.LoadLights(t_lights);
		m_animationShader.LoadViewMatrix(t_camera.GetViewMatrix());
		m_animationRenderer.Render(t_models);
		m_animationShader.UnbindShader();
	}
};