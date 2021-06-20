#include "GameOfLife.h"
#include "TextureLoader.h"

GameOfLife::GameOfLife() {
	m_texture1 = TextureLoader::LoadTexture("gameOfLifeStart.png", 1, 0.1, 0.1);
	glBindImageTexture(0, m_texture1.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	m_texture2 = TextureLoader::LoadTexture("gameOfLifeStart.png", 1, 0.1, 0.1);
	glBindImageTexture(1, m_texture2.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	m_shader = ComputeShader("..\\Res\\Shaders\\ComputeShaders\\gameOfLifeShader.txt");

	m_counter = 0;
}

Texture GameOfLife::run() {
	Texture currentTexture;
	Texture nextTexture;

	if (m_counter % 2 == 0) {
		currentTexture = m_texture1;
		nextTexture = m_texture2;
	}
	else {
		currentTexture = m_texture2;
		nextTexture = m_texture1;
	}

	glUseProgram(m_shader.m_programID);
	glBindImageTexture(0, currentTexture.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glBindImageTexture(1, nextTexture.GetTextureID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(4, 5, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	m_counter++;

 	return nextTexture;
}

Texture GameOfLife::getTexture() {
	return m_texture1;
}