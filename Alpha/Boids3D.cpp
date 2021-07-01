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
 	Quaternion rotationBase = Quaternion::RotationBetween3D(rayDirections[0], velocity);
	Vec3 normalVelocity = Vec3::Normalize(velocity);
	for (int i = 0; i < rayDirections.size(); i++) {
		Vec3 dir = Quaternion::RotatePoint(rotationBase, rayDirections[i]);
		Vec3 intersect = intersectPoint(dir, position, planeNormal, planePoint);
		if ((position - intersect).Length() > 50) {
 			return dir;
		}
	}

	return Vec3(1, 0, 0);
}

Vec3 intersectionWithSphere(Vec3& position, Vec3& velocity, Vec3& sphereCenter, float& sphereRadius) {
	Vec3 o = position;
	Vec3 c = sphereCenter;
	Vec3 u = Vec3::Normalize(velocity);

	float delta = pow(Vec3::Dot(u, (o - c)), 2) - (pow((o - c).Length(), 2) - sphereRadius * sphereRadius);
	float d = 0;
	if (abs(delta) < 0.0001) {
		d = -Vec3::Dot(u, (o - c));
	}
	else if (delta < 0) {
		return Vec3(-50000);
	}
	else {
		d = -Vec3::Dot(u, (o - c));
		float test = Vec3::Dot(u, (o - c));
		Vec3 point1 = u * (d + sqrt(delta));
		Vec3 point2 = u * (d - sqrt(delta));

		if ((o - point1).Length() < (o - point2).Length()) {
			return point1;
		}
		else {
			return point2;
		}
	}
}

Vec3 findDirectionSphere(std::vector<Vec3>& rayDirections, Vec3 position, Vec3 velocity, Vec3 sphereCenter, float sphereRadius) {
	Quaternion rotationBase = Quaternion::RotationBetween3D(rayDirections[0], velocity);
	Vec3 normalVelocity = Vec3::Normalize(velocity);
	for (int i = 0; i < rayDirections.size(); i++) {
		Vec3 dir = Quaternion::RotatePoint(rotationBase, rayDirections[i]);
		Vec3 intersect = intersectionWithSphere(position, velocity, sphereCenter, sphereRadius);
		if ((position - intersect).Length() > 50) {
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
	m_target = Vec3(50, 50, 50);
	for (int i = 0; i < agentCount; ++i) {
		Vec3 position = Vec3(Vec3(rand() % 100, rand() % 100, rand() % 100));
		
		Vec3 velocity = Vec3::Normalize(Vec3(random01(), random01(), random01())) * ((rand() % ((int)m_maxSpeed - 1)) + 1);
		if (velocity.x != velocity.x) {
			velocity = Vec3(1, 0, 0);
		}
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

	m_planes = std::vector<std::pair<Vec3, Vec3>>();
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(1, 0, 0), Vec3(0, 0, 0)));
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(1, 0, 0), Vec3(100, 0, 0)));
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 1, 0), Vec3(0, 0, 0)));
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 1, 0), Vec3(0, 100, 0)));
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 0, 1), Vec3(0, 0, 0)));
	m_planes.push_back(std::make_pair<Vec3, Vec3>(Vec3(0, 0, 1), Vec3(0, 0, 100)));

	std::vector<Vec3> torii = std::vector<Vec3>();
	torii.push_back(Vec3(25, 75, 25));
	torii.push_back(Vec3(75, 75, 75));
	torii.push_back(Vec3(50, 25, 50));
	
	for (int i = 0; i < 3; ++i) {
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(10, 0, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(-10, 0, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(0, 10, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(0, -10, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(0.7071 * 10, 0.7071 * 10, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(-0.7071 * 10, 0.7071 * 10, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(0.7071 * 10, -0.7071 * 10, 0), 0.5));
		m_spheres.push_back(std::make_pair<Vec3, float>(torii[i] + Vec3(-0.7071 * 10, -0.7071 * 10, 0), 0.5));
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
			acceleration += steerTowards(offsetToTarget, currentVelocity) * 500;
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

		for(int j = 0; j < m_planes.size(); ++j) {
			Vec3 intersect = intersectPoint(currentVelocity, currentPosition, m_planes[j].first, m_planes[j].second);
			if ((currentPosition - intersect).Length() < 30) {
  				Vec3 collisionAvoidDir = findDirection(m_directions, currentPosition, currentVelocity, m_planes[j].first, m_planes[j].second);
				Vec3 collisionAvoidForce = steerTowards(collisionAvoidDir, currentVelocity) * 10;
				acceleration += collisionAvoidForce;
			}

			Vec3 boidWallProjection = currentPosition - Vec3::Dot(currentPosition - m_planes[j].second, m_planes[j].first) * m_planes[j].first;
			Vec3 wallToBoid = currentPosition - boidWallProjection;
			if (wallToBoid.Length() > 0.01) {
				acceleration += Vec3::Normalize(wallToBoid) / wallToBoid.Length() * 100;
			}
		}

		for (int j = 0; j < m_spheres.size(); ++j) {
			Vec3 intersect = intersectionWithSphere(currentPosition, currentVelocity, m_spheres[j].first, m_spheres[j].second);
			if ((currentPosition - intersect).Length() < 30) {
				Vec3 collisionAvoidDir = findDirectionSphere(m_directions, currentPosition, currentVelocity, m_spheres[j].first, m_spheres[j].second);
				Vec3 collisionAvoidForce = steerTowards(collisionAvoidDir, currentVelocity) * 100;
				acceleration += collisionAvoidForce;
			}

			Vec3 boidSphereProjection = Vec3::Normalize(currentPosition - m_spheres[j].first) * m_spheres[j].second + m_spheres[j].first;
			Vec3 sphereToBoid = currentPosition - boidSphereProjection;
			if (sphereToBoid.Length() > 0.01) {
				acceleration += Vec3::Normalize(sphereToBoid) / sphereToBoid.Length() * 50;
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