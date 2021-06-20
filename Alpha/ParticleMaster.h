#pragma once
#include "Window.h"
#include "ParticleRenderer.h"

class ParticleMaster {

private:
	Window& m_window;

	ParticleShader m_particleShader = ParticleShader();
	ParticleRenderer m_particleRenderer;

public:
	ParticleMaster(Window& t_window, const Mat4& t_projectionMatrix) 
		: m_window(t_window), m_particleRenderer(m_particleShader) {
		m_particleShader.BindShader();
		m_particleShader.LoadProjectionMatrix(t_projectionMatrix);
		m_particleShader.LoadDrawDistance(DRAW_DISTANCE);
		m_particleShader.UnbindShader();
	}

	void Render(const std::map<GLuint, std::list<std::shared_ptr<ParticleEmitter>>>& t_emitters, Camera& t_camera) {
		m_particleShader.BindShader();
		m_particleRenderer.Render(t_emitters, t_camera);
		m_particleShader.UnbindShader();
	}

	void CleanUp() { m_particleShader.CleanUp(); }
};