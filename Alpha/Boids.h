#pragma once
#include <vector>
#include <memory>
#include "GUI.h"


class Boid {

public:
	Vec3 m_position;
	Vec3 m_velocity;
	std::shared_ptr<GUI> m_model;

public:
	Boid(Vec3 position, Vec3 velocity);

	void updatePosition();
	void updateVelocity(Vec3 newVelocity);

	std::shared_ptr<GUI> getModel();
	Vec3 getPosition();
	Vec3 getVelocity();
};

class Boids {

public:
	Texture m_texture;
	std::vector<Boid> m_agents;
	std::vector<std::shared_ptr<GUI>> m_barriers;
	float m_minSpeed;
	float m_maxSpeed;
	float m_minSteerForce;
	float m_maxSteerForce;
	bool m_enableTarget;

public:
	Boids(int agentCount);

	void run();
	Texture getTexture();
	Vec3 steerTowards(Vec3 destinationVector, Vec3 currentVector);
};