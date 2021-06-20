#pragma once
#include "Bone.h"
#include "Time.h"

class Animation {

private:
	Bone* m_root;
	float m_animationTime;
	float m_currentTime;
	std::string m_name;

public:
	Animation(Bone* t_root, float t_animationTime, std::string t_name) : m_root(t_root) {
		m_animationTime = t_animationTime;
		m_currentTime = 0.0f;
		m_name = t_name;
	}

	Bone* GetRootBone() { return m_root; }
	void SetAnimationTime(float t_time) { m_animationTime = t_time; }

	void Update(float t_frameTime) {
		m_currentTime += t_frameTime;
		while (m_currentTime >= m_animationTime) m_currentTime -= m_animationTime;

		m_root->Update(m_currentTime, nullptr);
	}
};