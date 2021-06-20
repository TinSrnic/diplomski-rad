#pragma once
#include "Entity.h"
#include "Math.h"
#include "WorldConstants.h"

/*Class to keep track of all things which are camera. That is a bad description.
But you can see what it does quite clearly, there is a bit of maths but it's mostly just geometry.
If you just draw the vectors out on paper you can get it quite quickly.*/
class Camera {

public:
	enum CameraType { FreeRoaming, Lock };

private:
	Vec3 m_position;
	Vec3 m_viewDirection;
	
	Mat4 m_translation;
	Mat4 m_rotation;

	float m_cameraSpeed; //value around 10 would be normal
	float m_cameraRotation; // value around 0.05 would be normal

	CameraType m_type;
	Vec3 m_offset; //offset from player if camera type lock is used
	Entity* m_trackTarget; //Entity to track

public:
	Camera(const Vec3& t_position, const Vec3& t_viewDirection, float t_cameraSpeed, float t_cameraRotation, const CameraType& t_type, const Vec3& t_offset, Entity* t_trackTarget = nullptr) {
		m_position = t_position;
		m_viewDirection = Vec3::Normalize(t_viewDirection);
		
		m_translation = Mat4::Translation(t_position);
		m_rotation = Mat4::RotationFromDirection(t_viewDirection);

		m_cameraSpeed = t_cameraSpeed;
		m_cameraRotation = t_cameraRotation;

		m_type = t_type;
		m_offset = t_offset;
		m_trackTarget = t_trackTarget;
	}

	Vec3 GetPosition() const { return m_position; }
	Vec3 GetViewDirection() const { return m_viewDirection; }
	Mat4 GetViewMatrix() const { return m_rotation * m_translation; }
	float GetCameraSpeed() const { return m_cameraSpeed; }
	float GetCameraRotation() const { return m_cameraRotation; }
	
	void SetPosition(const Vec3& t_position) {
		m_position = t_position;
		m_translation = Mat4::Translation(-t_position);
	}
	void SetViewDirection(const Vec3& t_viewDirection) {
		m_viewDirection = Vec3::Normalize(t_viewDirection);
		m_rotation = Mat4::RotationFromDirection(m_viewDirection);
	}
	void SetCameraSpeed(float t_cameraSpeed) { m_cameraSpeed = t_cameraSpeed; }
	void SetCameraRotation(float t_cameraRotation) { m_cameraRotation = t_cameraRotation; }
	void SetCameraType(const CameraType& t_type, Entity* t_trackTarget) {
		m_type = t_type;
		m_trackTarget = t_trackTarget;
	}
	void SetTrackTarget(Entity* t_trackTarget) {
		m_trackTarget = t_trackTarget;
	}

	void UpdateCamera(float t_mouseMoveX, float t_mouseMoveY, const Vec3& t_moveDirection) {
		//update view
		Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

		m_viewDirection = Mat4::Rotation(t_mouseMoveX * INVERT_X * m_cameraRotation, up) * m_viewDirection; //rotate view direction around y
		m_viewDirection = Mat4::Rotation(t_mouseMoveY * INVERT_Y * m_cameraRotation, Vec3::Cross(up, m_viewDirection)) * m_viewDirection; //rotate around current right

		m_rotation = Mat4::RotationFromDirection(m_viewDirection);
		
		//update position
		if (m_type == FreeRoaming || m_trackTarget == nullptr) {
			m_position = m_position + t_moveDirection * m_cameraSpeed;	
		} else if (m_type == Lock) {
			m_position = m_trackTarget->GetPosition() + m_offset;
		}
		
		//TODO: figure out why -m_position also works and in general it'd be a good idea to revise view/model matrix interaction
		m_translation = Mat4::Translation(-m_position);
	}
};