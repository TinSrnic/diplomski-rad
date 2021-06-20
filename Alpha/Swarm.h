#pragma once
#include "Texture.h"
#include "ComputeShader.h"


struct Agent {
	Vec2 m_position;
	float m_angle;
	Vec4i m_species;

	Agent();
	Agent(Vec2 position, float angle, Vec4i species);
};

class Swarm {

private:
	ComputeShader m_trailShader;
	ComputeShader m_diffuseShader;
	ComputeShader m_fadeShader;
	int m_width;
	int m_height;
	GLuint m_ssbo;
	long long m_agentCount;
	Texture m_texture;
	GLint m_trailDelta;
	GLint m_diffuseDelta;
	GLint m_fadeDelta;

public:
	Swarm(long long agentCount);

	void runTrail();
	void run();
	long long getAgentCount();
	Texture getTexture();
};