#pragma once
#include "Texture.h"
#include "ComputeShader.h"

class GameOfLife {

private:
	Texture m_texture1;
	Texture m_texture2;
	ComputeShader m_shader;
	int m_counter;

public:
	GameOfLife();
	Texture run();
	Texture getTexture();
};