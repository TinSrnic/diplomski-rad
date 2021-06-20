#pragma once
#include <vector>
#include "Texture.h"
#include "ComputeShader.h"

class RVOModel {

public:
	float robotRadius;
	std::vector<std::vector<float>> circularObstacles;
	std::vector<std::vector<float>> boundary;
};

class RvoAgent {
public:
	Vec2 m_position;
	Vec2 m_velocity;
	Vec2 m_goal;

	RvoAgent(Vec2 position, Vec2 velocity, Vec2 goal) {
		m_position = position;
		m_velocity = velocity;
		m_goal = goal;
	}

};

class RVO {

public:
	std::vector<RvoAgent> m_agents;
	ComputeShader m_computeShader;
	GLuint m_ssbo;
	GLfloat* m_ptr;
	Texture m_texture;

	Texture RVO::getTexture();

public:
	RVO(int agentCount);

	void run();

	static float distance(std::vector<float>& pos1, std::vector<float>& pos2);

	static std::vector<std::vector<float>> RVO_update(std::vector<std::vector<float>> x, std::vector<std::vector<float>> v_des, std::vector<std::vector<float>> v_current, RVOModel ws_model);

	static std::vector<float> intersect(std::vector<float> pA, std::vector<float> vA, std::vector<std::vector<std::vector<float>>> rvo_ba_all);

	static bool in_between(float theta_right, float theta_dif, float theta_left);

	static std::vector<std::vector<float>> compute_V_des(std::vector<std::vector<float>> x, std::vector<std::vector<float>> goal, std::vector<float> v_max);

	static bool reach(std::vector<float> p1, std::vector<float> p2, float bound = 0.5);
};

