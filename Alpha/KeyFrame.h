#pragma once
#include "Math.h"

/*Holds data for a transform of a bone at a certain time*/
class KeyFrame {
	
private:
	float m_timeStamp;
	Vec3 m_position;
	Quaternion m_rotation;

public:
	KeyFrame(float t_timeStamp, const Vec3& t_position, const Quaternion& t_rotation) : m_rotation(t_rotation) {
		m_timeStamp = t_timeStamp;
		m_position = t_position;
	}

	float GetTimeStamp() { return m_timeStamp; }
	Vec3 GetPosition() { return m_position; }
	Quaternion GetRotation() { return m_rotation; }

	void SetTimeStamp(float t_timeStamp) { m_timeStamp = t_timeStamp; }
	void SetPosition(const Vec3& t_position) { m_position = t_position; }
	void SetRotation(const Quaternion& t_rotation) { m_rotation = t_rotation; }
};