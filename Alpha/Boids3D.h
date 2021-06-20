#pragma once
#include <vector>
#include <memory>
#include "Entity.h"

class Boid3D {

public:
	Vec3 m_position;
	Vec3 m_velocity;
	std::shared_ptr<Entity> m_model;

public:
	Boid3D(Vec3 position, Vec3 velocity);

	void updatePosition();
	void updateVelocity(Vec3 newVelocity);

	std::shared_ptr<Entity> getModel();
	Vec3 getPosition();
	Vec3 getVelocity();
};

class Boids3D {

public:
	Texture m_texture;
	std::vector<Boid3D> m_agents;
	std::vector<Vec3> m_directions;
	float m_minSpeed;
	float m_maxSpeed;
	float m_minSteerForce;
	float m_maxSteerForce;
	bool m_enableTarget;
	Vec3 m_target;

public:
	Boids3D(int agentCount);

	void run();
	Texture getTexture();
	Vec3 steerTowards(Vec3 destinationVector, Vec3 currentVector);
};