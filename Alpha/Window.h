#pragma once
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "TextureLoader.h"
#include "Input.h"
#include "Display.h"

class Window {

private:
	GLFWwindow* m_window;
	float m_time = 0;
	float m_lastFrameTime = 0;
	std::string m_text = "";

public:
	int m_pressFrames[1024] = { 0 };
	bool m_pressRelease[1024] = { false };
	bool m_resetCursor = true;
	int m_scrollFrames = 0;
	bool m_noScroll = true;
	int m_height;
	int m_width;
	double m_cursorXpos, m_cursorYpos;
	double m_scrollXoffset, m_scrollYoffset;
	bool m_keys[1024], m_buttons[32];
	
public:
	Window(int width, int height) {
		m_width = width;
		m_height = height;

		Display::m_windowWidth = width;
		Display::m_windowHeight = height;

		m_cursorXpos = m_width / 2;
		m_cursorYpos = m_height / 2;

		if (!glfwInit()) {
			std::cout << "Failed to initilize GLFW!";
		}

		m_window = glfwCreateWindow(m_width, m_height, "Engine", NULL, NULL);
		if (!m_window) { return; }
		glfwMakeContextCurrent(m_window);
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initilize GLEW!";
		}

		unsigned int cursorWidth = 0;
		unsigned int cursorHeight = 0;
		BYTE* pixels(0);

		TextureLoader::LoadCursorTexture("SpaceCursor.png", m_window);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		glfwSetWindowPos(m_window, 0, 30);

		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
		glfwSetKeyCallback(m_window, KeyCallback);
		glfwSetCharCallback(m_window, CharacterCallback);
		glfwSetCursorPosCallback(m_window, CursorPosCallback);
		glfwSetMouseButtonCallback(m_window, &MouseButtonCallback);
		glfwSetScrollCallback(m_window, &ScrollCallback);
		m_time = (float)glfwGetTime();

		glfwSwapInterval(0);
	}

	std::string GetText() {
		std::string result = m_text;
		m_text.clear();
		return result;
	}

	void Clear() {
		glfwGetFramebufferSize(m_window, &m_width, &m_height);
		glViewport(0, 0, m_width, m_height);
		if (m_resetCursor) glfwSetCursorPos(m_window, m_width / 2.0, m_height / 2.0);
		if (!m_noScroll && m_scrollFrames == 0) {
			m_scrollFrames++;
		} else if (!m_noScroll && m_scrollFrames == 1) {
			m_noScroll = true;
			m_scrollFrames = 0;
			Input::m_mouseScroll = Vec2(0.0f);
		}

		for (int i = 0; i < 1024; ++i) {
			Input::m_previousFrameKeyPress[i] = false;
		}
	}
	void Update() {
		glfwSwapBuffers(m_window);
		glfwPollEvents();
		float currentTime = (float)glfwGetTime();
		m_lastFrameTime = currentTime - m_time;
		m_time = currentTime;
		m_scrollXoffset = 0.0f;
		m_scrollYoffset = 0.0f;
	}

	bool ShouldWindowClose() const {  return glfwWindowShouldClose(m_window) == 1;  }
	void Destroy() { glfwDestroyWindow(m_window); }

	GLFWwindow* GetWindow() const { return m_window; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }
	double GetAspect() const { return double(m_width) / m_height; }
	float GetTime() const { return m_time; }
	float GetFrameTime() const { return m_lastFrameTime; }

	static void WindowSizeCallback(GLFWwindow* window, int width, int height) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		if (width % 2 == 1) m_window->m_width = width - 1;
		else m_window->m_width = width;
		if (height % 2 == 1) m_window->m_height = height - 1;
		else m_window->m_height = height;

		Display::m_windowWidth = m_window->m_width;
		Display::m_windowHeight = m_window->m_height;

		glfwSetWindowSize(window, m_window->m_width, m_window->m_height);
	}
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		if (key == GLFW_KEY_ESCAPE && action != GLFW_RELEASE) glfwSetWindowShouldClose(window, true);
		if (action != GLFW_RELEASE) m_window->m_keys[key] = true;
		else m_window->m_keys[key] = false;
		//TODO: update all the code with this new and imporved mega ultra input shit
		if (action == GLFW_REPEAT) {
			Input::m_keyPress[key] = false;
			Input::m_keyHold[key] = true;
			Input::m_keyRelease[key] = false;
			Input::m_previousFrameKeyPress[key] = false;
		} else if (action == GLFW_PRESS) {
			Input::m_keyPress[key] = true;
			Input::m_keyHold[key] = false;
			Input::m_keyRelease[key] = false;
			Input::m_previousFrameKeyPress[key] = false;
		} else {
			Input::m_keyPress[key] = false;
			Input::m_keyHold[key] = false;
			Input::m_previousFrameKeyPress[key] = true;
			Input::m_keyRelease[key] = true;
		}
	}
	static void CharacterCallback(GLFWwindow* window, unsigned int codepoint) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		m_window->m_text += codepoint;
	}
	std::string GetClipboard() { return glfwGetClipboardString(m_window); }
	void SetClipboard(std::string string) { glfwSetClipboardString(m_window, string.c_str()); }
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		m_window->m_cursorXpos = xpos;
		m_window->m_cursorYpos = ypos;
		Input::m_mousePosition.x = (float)xpos;
		Input::m_mousePosition.y = (float)ypos;
	}
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		if (action != GLFW_RELEASE) m_window->m_buttons[button] = true;
		else m_window->m_buttons[button] = false;
		if (action == GLFW_PRESS) {
			Input::m_mousePress[button] = true;
		} else {
			Input::m_mousePress[button] = false;
		}
	}
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		Window* m_window = (Window*)glfwGetWindowUserPointer(window);
		m_window->m_scrollXoffset = xoffset;
		m_window->m_scrollYoffset = yoffset;
		Input::m_mouseScroll = Vec2((float)xoffset, (float)yoffset);
		m_window->m_noScroll = false;
	}
};
