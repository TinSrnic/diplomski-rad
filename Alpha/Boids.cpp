#include "Boids.h"
#include <algorithm>
#include "Time.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include "MousePickerOrthographic.h"

Boid::Boid(Vec3 position, Vec3 velocity) {
	m_position = position;
	m_velocity = velocity;

	Texture texture = Texture(TextureLoader::LoadTexture("triangle.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);	
	m_model = std::make_shared<GUI>(GUI(texturedModel, 0, "boid", m_position, Quaternion::RotationBetween(Vec3(0, 1, 0), m_velocity), Vec3(0.3), false, "asdf"));
}

void Boid::updatePosition() {
	m_position = m_position + m_velocity * Time::GetDeltaTime();
	m_position.x = fmod(m_position.x, 16.0f);
	m_position.y = fmod(m_position.y, 16.0f);

	if (m_position.x < 0.0f) m_position.x += 16.0f;
	if (m_position.y < 0.0f) m_position.y += 16.0f;
	m_model->SetPosition(m_position);
}

void Boid::updateVelocity(Vec3 newVelocity) {
	m_velocity = newVelocity;
	m_model->SetRotation(Quaternion::RotationBetween(Vec3(0, 1, 0), m_velocity));
}

std::shared_ptr<GUI> Boid::getModel() { return m_model; }
Vec3 Boid::getPosition() { return m_position; }
Vec3 Boid::getVelocity() { return m_velocity; }


Boids::Boids(int agentCount) {
	m_agents = std::vector<Boid>();
	m_minSpeed = 2;
	m_maxSpeed = 10;
	m_minSteerForce = 1;
	m_maxSteerForce = 5;
	m_enableTarget = true;
	for (int i = 0; i < agentCount; ++i) {
		Vec3 position = Vec3(((double)rand() / (RAND_MAX)) * 16, ((double)rand() / (RAND_MAX)) * 16, -5 + 3 * ((double)rand() / (RAND_MAX)));
		Vec3 velocity = Vec3::Normalize(Vec3(rand() % 16, rand() % 16, 0)) * ((rand() % ((int)m_maxSpeed - 1)) + 5);
		if (velocity.x != velocity.x) {
			velocity.x = 1.0f;
			velocity.y = 0.0f;
			velocity.z = 0.0f;
		}
		Boid newBoid = Boid(position, velocity);
		m_agents.push_back(newBoid);
	}

 	std::sort(m_agents.begin(), m_agents.end(),
		[](const Boid& a, const Boid& b) -> bool {
			return a.m_position.z > b.m_position.z;
		});

	m_texture = TextureLoader::LoadTexture("grey.png", 1, 0.1, 0.1);
}

void Boids::run() {
	for (int i = 0; i < m_agents.size(); ++i) {
		Vec3 currentPosition = m_agents[i].getPosition();
		currentPosition.z = -5;
		Vec3 currentVelocity = m_agents[i].getVelocity();
		
		int flockCount = 0;
		Vec3 flockCenter = Vec3(0);
		Vec3 flockDirection = Vec3(0);
		Vec3 flockPosition = Vec3(0);

		
		Vec3 acceleration = Vec3(0.0f);
		if (m_enableTarget) {
			Vec2 mouseTarget = MousePickerOrthographic::GetCurrentRay();
 			Vec3 offsetToTarget = (Vec3(mouseTarget, -5) - currentPosition);
			acceleration += steerTowards(offsetToTarget, currentVelocity);
		}

		for (int j = 0; j < m_agents.size(); ++j) {
			if (i == j) continue;
			Vec3 neighbourPosition = m_agents[j].getPosition();
			neighbourPosition.z = -5;
			Vec3 distanceVec = neighbourPosition - currentPosition;
			if (distanceVec.Length() > 2) continue;

			flockCount++;
			flockCenter += neighbourPosition;
			flockDirection += m_agents[j].m_velocity;
			flockPosition -= distanceVec;
		}

		if(flockCount != 0) {
			flockCenter /= flockCount;
			if (flockCenter != currentPosition) {
				Vec3 currentToCenter = flockCenter - currentPosition;
				flockDirection = flockDirection / flockCount;
				flockPosition = flockPosition / flockCount;

				Vec3 separationForce = steerTowards(flockPosition, currentVelocity) * 1.2;
				Vec3 alignmentForce = steerTowards(flockDirection, currentVelocity);
				Vec3 cohesionForce = steerTowards(currentToCenter, currentVelocity);

				acceleration += separationForce + alignmentForce + cohesionForce;

				currentVelocity = currentVelocity + acceleration * Time::GetDeltaTime();
				currentVelocity = Vec3::ClampMagnitude(currentVelocity, m_minSpeed, m_maxSpeed);
 				m_agents[i].updateVelocity(currentVelocity);
			}
		}

		m_agents[i].updatePosition();
	}
}

Texture Boids::getTexture() { return m_texture; }

Vec3 Boids::steerTowards(Vec3 destinationVector, Vec3 currentVector) {
	Vec3 v = Vec3::Normalize(destinationVector) * m_maxSpeed - currentVector;
	return Vec3::ClampMagnitude(v, m_minSteerForce, m_maxSteerForce);
}