#pragma once

class Time {
	friend class Engine;

private:
	static float m_time;
	static float m_deltaTime;

	static unsigned int m_frameCount;

public:
	static float m_timeScale;

	static float GetTime() { return m_time; }
	static float GetDeltaTime() { return m_deltaTime; }
	static unsigned int GetFrameCount() { return m_frameCount; }
};