#include "Boids3D.h"
#include "Time.h"
#include "TextureLoader.h"
#include "ModelLoader.h"

Boid3D::Boid3D(Vec3 position, Vec3 velocity) {
	m_position = position;
	m_velocity = velocity;

	Texture texture = Texture(TextureLoader::LoadTexture("orange.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("cone.obj"), texture, 0);
	m_model = std::make_shared<Entity>(Entity(texturedModel, 0, position, Quaternion::RotationBetween3D(Vec3(0, 0, -1), m_velocity), Vec3(1), Vec3(0), true, false, "asdf"));
}

Vec3 intersectPoint(Vec3 rayVector, Vec3 rayPoint, Vec3 planeNormal, Vec3 planePoint) {
	Vec3 diff = rayPoint - planePoint;
	double prod1 = Vec3::Dot(diff, planeNormal);
	double prod2 = Vec3::Dot(rayVector, planeNormal);
	double prod3 = prod1 / prod2;
	return rayPoint - rayVector * prod3;
}

Vec3 findDirection(std::vector<Vec3>& rayDirections, Vec3 position, Vec3 velocity, Vec3 planeNormal, Vec3 planePoint) {
	for (int i = 0; i < rayDirections.size(); i++) {
		Vec3 dir = rayDirections[i];
		Vec3 intersect = intersectPoint(dir + Vec3::Normalize(velocity), position, planeNormal, planePoint);
		if ((position - intersect).Length() > 15.0f) {
			return dir;
		}
	}

	return Vec3(1, 0, 0);
}

void Boid3D::updatePosition() {
 	m_position = m_position + m_velocity * Time::GetDeltaTime();
	m_position.x = fmod(m_position.x, 100);
	m_position.y = fmod(m_position.y, 100);
	m_position.z = fmod(m_position.z, 100);

	if (m_position.x < 0.0f) m_position.x += 100;
	if (m_position.y < 0.0f) m_position.y += 100;
	if (m_position.z < 0.0f) m_position.z += 100;

	m_model->SetPosition(m_position);
}

void Boid3D::updateVelocity(Vec3 newVelocity) {
	m_velocity = newVelocity;
	m_model->SetRotation(Quaternion::RotationBetween3D(Vec3(0, 0, -1), m_velocity));
}

std::shared_ptr<Entity> Boid3D::getModel() { return m_model; }
Vec3 Boid3D::getPosition() { return m_position; }
Vec3 Boid3D::getVelocity() { return m_velocity; }


float random01() {
	return ((double)rand() / (RAND_MAX));
}

Boids3D::Boids3D(int agentCount) {
	m_agents = std::vector<Boid3D>();
	m_minSpeed = 10;
	m_maxSpeed = 40;
	m_minSteerForce = 0;
	m_maxSteerForce = 20;
	m_enableTarget = true;
	m_target = Vec3(25, 75, 40);
	/*for (int i = 0; i < agentCount; ++i) {
		Vec3 position = Vec3(Vec3(rand() % 100, rand() % 100, rand() % 100));
		Vec3 velocity = Vec3::Normalize(Vec3(random01(), random01(), random01())) * ((rand() % ((int)m_maxSpeed - 1)) + 1);
		if (velocity.x != velocity.x) {
			velocity = Vec3(1, 0, 0);
		}
		Boid3D newBoid = Boid3D(position, velocity);
		m_agents.push_back(newBoid);
	}*/

	for (int i = 0; i < 100; ++i) {
		Vec3 position = Vec3(50.0f, 50.0f, 100.0f) + Vec3(-25 * i, 25 * i, -75 * i) / 40.0f;
		Vec3 velocity = Vec3::Normalize(Vec3(-25, 25, -75));

		position += Vec3(random01(), random01(), random01()) * 3;

		Boid3D newBoid = Boid3D(position, velocity);
		m_agents.push_back(newBoid);
	}
	for (int i = 100; i < 200; ++i) {
		Vec3 position = Vec3(50.0f, 50.0f, 100.0f) + Vec3(25 * (i - 100), 25 * (i - 100), -25 * (i - 100)) / 40.0f;
		position += Vec3(random01(), random01(), random01()) * 3;
		Vec3 velocity = Vec3::Normalize(Vec3(25, 25, -25));
		Boid3D newBoid = Boid3D(position, velocity);
		m_agents.push_back(newBoid);
	}
	for (int i = 200; i < 300; ++i) {
		Vec3 position = Vec3(50.0f, 50.0f, 100.0f) + Vec3(0, -25 * (i - 200), -50 * (i - 200)) / 40.0f;
		position += Vec3(random01(), random01(), random01()) * 3;
		Vec3 velocity = Vec3::Normalize(Vec3(0, -25, -50));
		Boid3D newBoid = Boid3D(position, velocity);
		m_agents.push_back(newBoid);
	}

	int numViewDirections = 300;
	m_directions = std::vector<Vec3>();

	float goldenRatio = (1 + sqrt(5)) / 2;
	float angleIncrement = M_PI * 2 * goldenRatio;

	for (int i = 0; i < numViewDirections; i++) {
		float t = (float)i / numViewDirections;
		float inclination = acos(1 - 2 * t);
		float azimuth = angleIncrement * i;

		float x = sin(inclination) * cos(azimuth);
		float y = sin(inclination) * sin(azimuth);
		float z = cos(inclination);
		m_directions.push_back(Vec3(x, y, z));
	}

	m_texture = TextureLoader::LoadTexture("black.png", 1, 0.1, 0.1);
}

void Boids3D::run() {
	for (int i = 0; i < m_agents.size(); ++i) {
		Vec3 currentPosition = m_agents[i].getPosition();
		Vec3 currentVelocity = m_agents[i].getVelocity();

		int flockCount = 0;
		Vec3 flockCenter = Vec3(0);
		Vec3 flockDirection = Vec3(0);
		Vec3 flockPosition = Vec3(0);
		for (int j = 0; j < m_agents.size(); ++j) {
			if (i == j) continue;
			Vec3 neighbourPosition = m_agents[j].getPosition();
			Vec3 distanceVec = neighbourPosition - currentPosition;
			if (distanceVec.Length() > 20) continue;

			flockCount++;
			flockCenter += neighbourPosition;
			flockDirection += m_agents[j].m_velocity;
			flockPosition -= distanceVec;
		}

		Vec3 acceleration = Vec3(0);
		
		if (m_enableTarget) {
			Vec3 offsetToTarget = (m_target - currentPosition);
			acceleration += steerTowards(offsetToTarget, currentVelocity) * 30;
		}

		if (flockCount != 0) {
			flockCenter /= flockCount;
			if (flockCenter != currentPosition) {
				Vec3 currentToCenter = flockCenter - currentPosition;
				flockDirection = flockDirection / flockCount;
				flockPosition = flockPosition / flockCount;

				Vec3 separationForce = steerTowards(flockPosition, currentVelocity);
				Vec3 alignmentForce = steerTowards(flockDirection, currentVelocity);
				Vec3 cohesionForce = steerTowards(currentToCenter, currentVelocity);

				acceleration = separationForce + alignmentForce + cohesionForce;
			}
		}

		std::vector<std::pair<Vec3, Vec3>> planes = std::vector<std::pair<Vec3, Vec3>>();
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(1, 0, 0), Vec3(0, 0, 0)));
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(1, 0, 0), Vec3(100, 0, 0)));
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 1, 0), Vec3(0, 0, 0)));
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 1, 0), Vec3(0, 100, 0)));
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 0, 1), Vec3(0, 0, 0)));
		planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 0, 1), Vec3(0, 0, 100)));

		for(int j = 0; j < planes.size(); ++j) {
			Vec3 intersect = intersectPoint(currentVelocity, currentPosition, planes[j].first, planes[j].second);
			if ((currentPosition - intersect).Length() < 10) {
  				Vec3 collisionAvoidDir = findDirection(m_directions, currentPosition, currentVelocity, planes[j].first, planes[j].second);
				Vec3 collisionAvoidForce = steerTowards(collisionAvoidDir, currentVelocity) * 100;
				acceleration += collisionAvoidForce;
			}
		}
		
		currentVelocity = currentVelocity + acceleration * Time::GetDeltaTime();
		currentVelocity = Vec3::ClampMagnitude(currentVelocity, m_minSpeed, m_maxSpeed);
		m_agents[i].updateVelocity(currentVelocity);
		m_agents[i].updatePosition();
	}
}

Texture Boids3D::getTexture() { return m_texture; }

Vec3 Boids3D::steerTowards(Vec3 destinationVector, Vec3 currentVector) {
	Vec3 v = Vec3::Normalize(destinationVector) * m_maxSpeed - currentVector;
	return Vec3::ClampMagnitude(v, m_minSteerForce, m_maxSteerForce);
}