#pragma once
#include <list>
#include <string>
#include "KeyFrame.h"

class Bone {

private:
	std::list<Bone*> m_children = std::list<Bone*>();
	std::vector<KeyFrame> m_keyFrames = std::vector<KeyFrame>();
	int m_index;
	std::string m_name;
	Mat4 m_currentTransform; //model space transform of the bone, by updating this you change the pose of the model
	Mat4 m_startTransform; //inverse transform in model space (in reference to origin of the entire model)

public:
	Bone(int t_index, const std::string& t_name, const Mat4& t_startTransform) {
		m_index = t_index;
		m_name = t_name;
		m_startTransform = t_startTransform;
		m_currentTransform = Mat4::Invert(t_startTransform);
	}

	void AddChild(Bone* t_child) { m_children.push_back(t_child); }
	void AddKeyFrame(KeyFrame& t_keyFrame) { m_keyFrames.push_back(t_keyFrame); }

	int GetIndex() { return m_index; }
	Mat4 GetInverseStartTransform() { return Mat4::Invert(m_startTransform); }
	Mat4 GetStartTransform() { return m_startTransform; }
	Mat4 GetCurrentTransform() { return m_currentTransform; }
	KeyFrame* GetKeyFrame(int t_index) { return &(m_keyFrames.at(t_index)); }
	std::list<Bone*>& GetChildren() { return m_children; }
	std::string GetName() const { return m_name; }

	void SetName(const std::string& t_name) { m_name = t_name; }
	void SetStartTransform(const Mat4& t_startTransform) { m_startTransform = t_startTransform; }

	void Update(float t_currentTime, const Mat4* t_parentTransform) {
		for (std::vector<KeyFrame>::iterator it = m_keyFrames.begin(); it != m_keyFrames.end(); ++it) {
			if ((*it).GetTimeStamp() > t_currentTime) {
				KeyFrame& nextFrame = (*it);
				KeyFrame& previousFrame = (*(--it));
				float blend = (t_currentTime - previousFrame.GetTimeStamp()) / (nextFrame.GetTimeStamp() - previousFrame.GetTimeStamp());

				if (t_parentTransform == nullptr) {
					m_currentTransform = Mat4::Translation(previousFrame.GetPosition() + (nextFrame.GetPosition() - previousFrame.GetPosition()) * blend) * Quaternion::Slerp(previousFrame.GetRotation(), nextFrame.GetRotation(), blend).ConvertToMat4();
				} else {
					m_currentTransform = *t_parentTransform * Mat4::Translation(previousFrame.GetPosition() + (nextFrame.GetPosition() - previousFrame.GetPosition()) * blend) * Quaternion::Slerp(previousFrame.GetRotation(), nextFrame.GetRotation(), blend).ConvertToMat4();
				}

				for each (Bone* child in m_children) {
					child->Update(t_currentTime, &m_currentTransform);
				}

				return;
			}
		}
	}
};