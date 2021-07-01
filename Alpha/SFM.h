#pragma once
#include <vector>
#include <memory>
#include "Math.h"
#include "GUI.h"
#include "TextureLoader.h"
#include "ModelLoader.h"

class SFMAgent {

public:
	Vec3 m_position;
	Vec3 m_goal;
	Vec3 m_direction;
	float m_size;
	std::shared_ptr<GUI> m_model;

public:
	SFMAgent(Vec3 position, Vec3 goal, float size, std::string textureName, Vec4 color);
	SFMAgent::SFMAgent(Vec3 position, Vec3 goal, float size, std::string textureName);

	void updatePosition(Vec3 direction);
	std::shared_ptr<GUI> getModel();
};

class SFM {

public:
	std::vector<SFMAgent> m_agents;
	std::vector<std::shared_ptr<GUI>> m_barriers;
	Texture m_texture;
	float m_maxSpeed;

public:
	SFM(int agentCount);

	void run();
	Texture getTexture();
};

