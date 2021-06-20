#pragma once
#include <glfw3.h>
#include <iostream>

class FPSCounter {

private:
	unsigned int m_lastNbFrames = 0;
	unsigned int m_nbFrames;
	double m_lastTime;
	double m_currentTime;
	double m_printSpeed;
	bool m_ready = false;

public:
	FPSCounter(double printSpeed) {
		m_lastTime = glfwGetTime();
		m_printSpeed = printSpeed;
		m_nbFrames = 0;
	}

	void Time() {
		m_currentTime = glfwGetTime();
		m_nbFrames++;

		if (m_currentTime - m_lastTime >= m_printSpeed) { 
			m_lastNbFrames = m_nbFrames;
			m_nbFrames = 0;
			m_lastTime += 1.0;
		}
	}

	unsigned int GetFrames() {
		return m_lastNbFrames;
	}


};