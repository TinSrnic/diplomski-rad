#include "Swarm.h"
#include "TextureLoader.h"
#include "Time.h"
#include "CustomRandom.h"
#include <iostream>

Agent::Agent() {
	m_position = Vec2(0);
	m_angle = 0.0f;
	m_species = Vec4i(0);
}

Agent::Agent(Vec2 position, float angle, Vec4i species) {
	m_position = position;
	m_angle = angle;
	m_species = species;
}

long long Swarm::getAgentCount() {
	return m_agentCount;
}

Swarm::Swarm(long long agentCount) {
	m_agentCount = agentCount;

	m_texture = TextureLoader::LoadTexture("black.png", 1, 0.1, 0.1);
	glBindImageTexture(0, m_texture.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	m_width = m_texture.GetWidth();
	m_height = m_texture.GetHeight();

	std::vector<Agent> agents = std::vector<Agent>();
	Vec2 center = Vec2(m_width / 2, m_height / 2);
	CustomRandom random = CustomRandom(42);

	for (int i = 0; i < agentCount; ++i) {
		float randomNumber = rand();
		Vec2 agentPosition = center + Vec2(cos(randomNumber), sin(randomNumber)) * random.GetNoise() * 500;
		//float agentAngle = atan2(center.y - agentPosition.y, center.x - agentPosition.x);
		float agentAngle = ((double)rand() / (RAND_MAX)) * 2 * M_PI;

		int randSpecies = rand() % 3;
		//agents.push_back(Agent(agentPosition, agentAngle, Vec4i(1, 1, 1, 0)));
		if (randSpecies == 0) {
			agents.push_back(Agent(agentPosition, agentAngle, Vec4i(1, 0, 0, 0)));
		} else if (randSpecies == 1) {
			agents.push_back(Agent(agentPosition, agentAngle, Vec4i(0, 1, 0, 0)));
		} else if (randSpecies == 2) {
			agents.push_back(Agent(agentPosition, agentAngle, Vec4i(0, 0, 1, 0)));
		}
	}

	GLfloat* buffer = new GLfloat[agentCount * 8];

	for (int i = 0; i < agents.size(); ++i) {
		buffer[i * 8 + 0] = agents[i].m_position.x;
		buffer[i * 8 + 1] = agents[i].m_position.y;
		buffer[i * 8 + 2] = agents[i].m_angle;
		buffer[i * 8 + 3] = 0.0f;
		buffer[i * 8 + 4] = agents[i].m_species.x;
		buffer[i * 8 + 5] = agents[i].m_species.y;
		buffer[i * 8 + 6] = agents[i].m_species.z;
		buffer[i * 8 + 7] = agents[i].m_species.w;
	}

	glGenBuffers(1, &m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_agentCount * 8 * sizeof(GLfloat), buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	m_trailShader = ComputeShader("..\\Res\\Shaders\\ComputeShaders\\trailShader.txt");
	m_diffuseShader = ComputeShader("..\\Res\\Shaders\\ComputeShaders\\diffuseShader.txt");
	m_fadeShader = ComputeShader("..\\Res\\Shaders\\ComputeShaders\\fadeShader.txt");
	GLint trailWidth = glGetUniformLocation(m_trailShader.m_programID, "width");
	GLint diffuseWidth = glGetUniformLocation(m_diffuseShader.m_programID, "width");
	GLint trailHeight = glGetUniformLocation(m_trailShader.m_programID, "height");
	GLint diffuseHeight = glGetUniformLocation(m_diffuseShader.m_programID, "height");

	glUseProgram(m_trailShader.m_programID);
	glUniform1i(trailWidth, m_width);
	glUniform1i(trailHeight, m_height);

	glUseProgram(m_diffuseShader.m_programID);
	glUniform1i(diffuseWidth, m_width);
	glUniform1i(diffuseHeight, m_height);

	m_trailDelta = glGetUniformLocation(m_trailShader.m_programID, "deltaTime");
	m_diffuseDelta = glGetUniformLocation(m_diffuseShader.m_programID, "deltaTime");
	m_fadeDelta = glGetUniformLocation(m_fadeShader.m_programID, "deltaTime");
}

void Swarm::runTrail() {
	glUseProgram(m_trailShader.m_programID);
	glUniform1f(m_trailDelta, 0.1);
	glDispatchCompute(m_agentCount / 64, 1, 1);
}

void Swarm::run() {
	glUseProgram(m_trailShader.m_programID);
	glUniform1f(m_trailDelta, Time::GetDeltaTime());
	glDispatchCompute(m_agentCount / 64, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glUseProgram(m_diffuseShader.m_programID);
	glUniform1f(m_diffuseDelta, Time::GetDeltaTime());
	glDispatchCompute(m_width / 8, m_height / 8, 1);
	
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
	glUseProgram(m_fadeShader.m_programID);
	glUniform1f(m_fadeDelta, Time::GetDeltaTime());
	glDispatchCompute(m_width / 8, m_height / 8, 1);
	
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

Texture Swarm::getTexture() { return m_texture; }