#pragma once
#include "SFM.h"
#include "Time.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include <algorithm>

SFMAgent::SFMAgent(Vec3 position, Vec3 goal, float size, std::string textureName, Vec4 color) {
	m_position = position;
	m_goal = goal;
	m_size = size;
	m_direction = Vec3::Normalize(goal - position);
	Texture texture = Texture(TextureLoader::LoadTexture(textureName), 1, 0.1f, 0.1f);
	TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);
	m_model = std::make_shared<GUI>(GUI(texturedModel, 0, "boid", m_position, Quaternion::Identity(), Vec3(0.3), false, "asdf", color));
}

float randomNumber() {
	return ((float)rand() / (RAND_MAX));
}

void SFMAgent::updatePosition(Vec3 direction) {
	m_position = m_position + direction * 0.1 * Time::GetDeltaTime();
	m_direction = Vec3::Normalize(m_direction + direction * 0.1 * Time::GetDeltaTime());
	m_model->SetPosition(m_position);
	m_model->SetRotation(Quaternion::RotationBetween(Vec3(0, 1, 0), m_direction));
}

std::shared_ptr<GUI> SFMAgent::getModel() { return m_model; }

SFM::SFM(int agentCount) {
	m_maxSpeed = 10;
	Vec3 goal1 = Vec3(100, 8, -5);
	Vec3 goal2 = Vec3(1, 8, -5);
	for (int i = 0; i < agentCount; ++i) {
		if (i < agentCount / 2 || true) {
			Vec3 position = Vec3(randomNumber() * 2 - 5, randomNumber() * 2 + 7, -5 + randomNumber() * 3);
			m_agents.push_back(SFMAgent(position, goal1, 3.0, "sheep.png", Vec4(250 / 255.0f, 218 /255.0f, 94 / 255.0f, 1.0f)));
		} else {
			Vec3 position = Vec3(randomNumber() * 2 + 11, randomNumber() * 2 + 7, -5 + randomNumber() * 3);
			m_agents.push_back(SFMAgent(position, goal2, 3.0, "sheep.png", Vec4(1, 1, 1, 1)));
		}
	}

	std::sort(m_agents.begin(), m_agents.end(),
		[](const SFMAgent& a, const SFMAgent& b) -> bool {
			return a.m_position.z > b.m_position.z;
		});

	m_texture = TextureLoader::LoadTexture("grass.png", 1, 0.1, 0.1);

	Texture texture = Texture(TextureLoader::LoadTexture("brown.png"), 1, 0.1f, 0.1f);
	TexturedModel texturedModel = TexturedModel(ModelLoader::LoadModel("plane.obj"), texture, 0);
	/*m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 0, -4), Quaternion::Identity(), Vec3(8, 2, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 16, -4), Quaternion::Identity(), Vec3(8, 2, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 6.5 / 2, -4), Quaternion::Rotation(90, Vec3(0,0,1)), Vec3(6.5 / 2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 16 - 6.5 / 2, -4), Quaternion::Rotation(90, Vec3(0,0,1)), Vec3(6.5 / 2, 0.1, 1), false, "asdf")));*/

	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 0, -4), Quaternion::Identity(), Vec3(16, 2, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(8, 16, -4), Quaternion::Identity(), Vec3(16, 2, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(3, 4, -4), Quaternion::Identity(), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(4, 6, -4), Quaternion::Identity(), Vec3(2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(9.5, 4, -4), Quaternion::Identity(), Vec3(2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(9.8, 6, -4), Quaternion::Identity(), Vec3(2.2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(6.8, 8, -4), Quaternion::Identity(), Vec3(0.8, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(5, 10, -4), Quaternion::Identity(), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(13, 10, -4), Quaternion::Identity(), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(12, 12, -4), Quaternion::Identity(), Vec3(2, 0.1, 1), false, "asdf")));

	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(2.1, 3, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(5.9, 5, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(13.9, 13, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(1, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(2.1, 10, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(4, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(7.6, 8, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(4, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(4.1, 10, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(5.9, 12, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(13.9, 6, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(4, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(10.1, 10, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(2, 0.1, 1), false, "asdf")));
	m_barriers.push_back(std::make_shared<GUI>(GUI(texturedModel, 0, "boid", Vec3(12.1, 8, -4), Quaternion::Rotation(90, Vec3(0, 0, 1)), Vec3(2, 0.1, 1), false, "asdf")));
}

void SFM::run() {
	Vec3 result = Vec3(0);
	for (int i = 0; i < m_agents.size(); ++i) {
		Vec3 totalForce = Vec3(0);
		for (int j = 0; j < m_agents.size(); ++j) {
			if (i == j) continue;
			Vec3 agentIPos = m_agents[i].m_position;
			Vec3 agentJPos = m_agents[j].m_position;
			agentIPos.z = -5;
			agentJPos.z = -5;
			Vec3 distanceVec = agentIPos - agentJPos;
			if (distanceVec.Length() < 2) {
				totalForce += (distanceVec / (distanceVec.Length() * distanceVec.Length())) * 8;
			}
		}

		for (int j = 0; j < m_barriers.size(); ++j) {
			Vec3 closestPoint = m_agents[i].m_position;
			closestPoint.z = -5;
			Vec3 barrierPosition = m_barriers[j]->GetPosition();
			Vec3 barrierScale = m_barriers[j]->GetScale();
			Vec3 min = Vec3(0);
			Vec3 max = Vec3(0);
			if (m_barriers[j]->GetRotation() == Quaternion::Identity()) {
				min = barrierPosition - barrierScale;
				max = barrierPosition + barrierScale;
			} else {
				float temp = barrierScale.x;
				barrierScale.x = barrierScale.y;
				barrierScale.y = temp;
				min = barrierPosition - barrierScale;
				max = barrierPosition + barrierScale;
			}
			
			if (closestPoint.x < min.x) closestPoint.x = min.x;
			else if (closestPoint.x > max.x) closestPoint.x = max.x;
			if (closestPoint.y < min.y) closestPoint.y = min.y;
			else if (closestPoint.y > max.y) closestPoint.y = max.y;
			closestPoint.z = -5;

			Vec3 tempPosition = m_agents[i].m_position;
			tempPosition.z = -5;
			Vec3 distanceVec = tempPosition - closestPoint;
			if (distanceVec.Length() < 0.5) {
				totalForce += (distanceVec / (distanceVec.Length() * distanceVec.Length())) * 16;
			}
		}

		Vec3 tempPosition = m_agents[i].m_position;
		tempPosition.z = -5;

		totalForce += Vec3::Normalize(m_agents[i].m_goal - tempPosition) * m_maxSpeed;
		m_agents[i].updatePosition(totalForce);
	}
	
}

Texture SFM::getTexture() {
	return m_texture;
}