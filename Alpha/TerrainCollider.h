#pragma once
#include "BoundedBody.h"
#include "Force.h"

class TerrainCollider {

private:


public:
	TerrainCollider() {

	}

	Vec3 IsColliding(Vec3& moment, const std::shared_ptr<BoundedBody>& entity) {
		moment = Vec3(0.0f);
		Vec3 position = entity->GetPosition();
		Quaternion rotation = entity->GetRotation();
		Vec3 scale = entity->GetScale();

		//need 3 points to see what rotation is needed
		Vec3 points[8];
		points[0] = position + Quaternion::RotatePoint(rotation, Vec3(-scale.x, -scale.y, -scale.z));
		points[1] = position + Quaternion::RotatePoint(rotation, Vec3(scale.x, -scale.y, -scale.z));
		points[2] = position + Quaternion::RotatePoint(rotation, Vec3(scale.x, -scale.y, scale.z));
		points[3] = position + Quaternion::RotatePoint(rotation, Vec3(-scale.x, -scale.y, scale.z));
		points[4] = position + Quaternion::RotatePoint(rotation, Vec3(-scale.x, scale.y, -scale.z));
		points[5] = position + Quaternion::RotatePoint(rotation, Vec3(scale.x, scale.y, -scale.z));
		points[6] = position + Quaternion::RotatePoint(rotation, Vec3(scale.x, scale.y, scale.z));
		points[7] = position + Quaternion::RotatePoint(rotation, Vec3(-scale.x, scale.y, scale.z));

		float heights[8];
		heights[0] = points[0].y - GameObjects::GetTerrainHeight(Vec2(points[0].x, points[0].z));
		heights[1] = points[1].y - GameObjects::GetTerrainHeight(Vec2(points[1].x, points[1].z));
		heights[2] = points[2].y - GameObjects::GetTerrainHeight(Vec2(points[2].x, points[2].z));
		heights[3] = points[3].y - GameObjects::GetTerrainHeight(Vec2(points[3].x, points[3].z));
		heights[4] = points[4].y - GameObjects::GetTerrainHeight(Vec2(points[4].x, points[4].z));
		heights[5] = points[5].y - GameObjects::GetTerrainHeight(Vec2(points[5].x, points[5].z));
		heights[6] = points[6].y - GameObjects::GetTerrainHeight(Vec2(points[6].x, points[6].z));
		heights[7] = points[7].y - GameObjects::GetTerrainHeight(Vec2(points[7].x, points[7].z));

		int smallestIndex = 0;
		for (int i = 0; i < 8; ++i) {
			if(heights[i] < heights[smallestIndex]) smallestIndex = i;
			if (heights[i] < 0.0f) {
				moment += Vec3::Cross(points[i] - position, Vec3(0.0f, -heights[i], 0.0f));
			}
		}

		if (heights[smallestIndex] < 0.0f) {
			return Vec3(0.0f, -heights[smallestIndex], 0.0f);
		} else {
			return Vec3(0.0f);
		}
	}

};