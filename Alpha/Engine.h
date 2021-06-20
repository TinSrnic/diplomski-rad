#pragma once
#include "ProcessInfo.h"
#include "MasterRenderer.h"
#include "GUIMaster.h"
#include "TextMaster.h"
#include "ShadowMapMaster.h"
#include "LineMaster.h"
#include "FPSCounter.h"
#include "Time.h"
#include "StringManipulation.h"
#include "GameObjects.h"
#include "MarchingCubeRenderer.h"
#include "MousePickerOrthographic.h"

class Engine {

public:
	GLuint GetShadowMap() { return m_shadowMapMaster.GetShadowMap(); }

	Window m_window;
	Camera* m_camera;

private:
	bool m_displayInfo = false;
	
	bool m_updateCameraFlag = false;

	MasterRenderer m_masterRenderer;
	GUIMaster m_guiMaster;
	TextMaster m_textMaster;
	LineMaster m_lineMaster;
	MarchingCubeRenderer m_marchingCubeRenderer;

public:
	bool m_mouseUnlock = false;
	ShadowMapMaster m_shadowMapMaster;
private:
	Mat4 m_perspectiveMatrix;
	Mat4 m_orthographicMatrix;

	ProcessInfo m_process = ProcessInfo();
	FPSCounter m_fps = FPSCounter(1.0);

public:
	Engine(const Mat4& t_perspectiveMatrix, const Mat4& t_orthographicMatrix, unsigned int t_screenWidth, unsigned int t_screenHeight, Camera* t_camera)
		: m_window(t_screenWidth, t_screenHeight), m_camera(t_camera), m_lineMaster(t_orthographicMatrix), m_guiMaster(t_orthographicMatrix), m_textMaster(t_orthographicMatrix),
		m_masterRenderer(t_perspectiveMatrix), m_shadowMapMaster(m_window, *m_camera), m_marchingCubeRenderer(t_perspectiveMatrix) {

		World::m_orthographicMatrix = t_orthographicMatrix;
		World::m_perspectiveMatrix = t_perspectiveMatrix;

		srand((unsigned int)time(NULL));
		//gl variable setup
		glEnable(GL_DEPTH_TEST); //to acctually be able to render 3d stuff
		//glEnable(GL_CULL_FACE); //speed increase don't render the back of things which usually can't be seen
		//glCullFace(GL_BACK);
		glClearColor(70.0f / 255.0f, 65.0f / 255.0f, 61.0f / 255.0f, 1.0);

		m_perspectiveMatrix = t_perspectiveMatrix;
		m_orthographicMatrix = t_orthographicMatrix;
	}

	bool ShouldStop() { return m_window.ShouldWindowClose(); }
	void Render(std::list<std::shared_ptr<Terrain>>& t_terrains, std::shared_ptr<Skybox>& t_skybox, std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities,
		std::vector<std::shared_ptr<Line>>& t_lines, std::map<GLuint, std::vector<std::shared_ptr<GUI>>> t_GUIs, std::map<GLuint, std::vector<std::shared_ptr<Text>>> t_texts, const std::vector<Light>& t_lights) {

		UpdateTime();
		if (m_updateCameraFlag) {
			UpdateCamera();
		}
		if (Input::GetKeyPress(GLFW_KEY_U)) {
			m_mouseUnlock = !m_mouseUnlock;
			if (m_mouseUnlock) {
				m_window.m_resetCursor = false;
				glfwSetInputMode(m_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			} else {
				m_window.m_resetCursor = false;
				glfwSetInputMode(m_window.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}
		m_window.Clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MousePickerOrthographic::Update();

		UpdateShadows(t_terrains, t_entities, t_lights.at(0));
		m_process.Update();

		//render
		m_masterRenderer.Render(t_terrains, t_skybox, t_entities, t_lights, *m_camera, m_shadowMapMaster.GetToShadowMapSpaceMatrix(), m_shadowMapMaster.GetShadowMap(), m_window.GetTime());
		m_lineMaster.Render(t_lines, m_camera->GetViewMatrix());
		m_textMaster.Render(t_texts);
		m_guiMaster.Render(t_GUIs, t_lights.at(0));
	}
	void UpdateWindow() {
		m_window.Update();
	}
	void Stop() {
		m_masterRenderer.CleanUp();
		m_shadowMapMaster.CleanUp();
		m_guiMaster.CleanUp();
		m_textMaster.CleanUp();

		glfwTerminate();
	}

	float GetFrameTime() { return m_window.GetFrameTime(); }
	float GetTime() { return m_window.GetTime(); }

	void SetCamera(Camera* t_camera) {
		m_camera = t_camera;
	}

	void EnableClipping() { glEnable(GL_CLIP_DISTANCE0); }
	void DisableClipping() { glDisable(GL_CLIP_DISTANCE0); }
	void LoadClipPlane(const Vec4& t_clipPlane) {
		m_masterRenderer.LoadClipPlane(t_clipPlane);
	}

	void InvertCamera() {
		Vec3 cameraPosition = m_camera->GetPosition();
		float cameraHeight = WATER_HEIGHT - cameraPosition.y;
		m_camera->SetPosition(Vec3(cameraPosition.x, cameraHeight, cameraPosition.z));
		Vec3 viewPleasure = m_camera->GetViewDirection();
		viewPleasure.y = -viewPleasure.y;
		m_camera->SetViewDirection(viewPleasure);
	}
	void setCameraUpdate(bool update) {
		m_updateCameraFlag = update;
	}

private:
	void UpdateTime() {
		Time::m_time = m_window.GetTime();
		Time::m_deltaTime = m_window.GetFrameTime();
		m_fps.Time();
		Time::m_frameCount = m_fps.GetFrames();
	}
	void UpdateCamera() {
		//update view
		float mouseMoveX;
		float mouseMoveY;

		if (!m_mouseUnlock) {
			mouseMoveX = (float)((m_window.m_cursorXpos - m_window.m_width / 2.0f)); //get how much you should move to the right or left
			mouseMoveY = (float)((m_window.m_cursorYpos - m_window.m_height / 2.0f)); //get how much you should move up or down
			if (abs(mouseMoveX) < DEAD_ZONE) mouseMoveX = 0.0f;
			if (abs(mouseMoveY) < DEAD_ZONE) mouseMoveY = 0.0f;
		}
		else {
			mouseMoveX = 0.0f;
			mouseMoveY = 0.0f;
		}

		//update position
		Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 move = Vec3(0.0f);
		Vec3 cameraView = m_camera->GetViewDirection();
		if (m_window.m_keys[GLFW_KEY_SPACE]) {
			move += up;
		}
		if (m_window.m_keys[GLFW_KEY_LEFT_SHIFT]) {
			move -= up;
		}
		if (m_window.m_keys[GLFW_KEY_W]) {
			move += cameraView;
		}
		if (m_window.m_keys[GLFW_KEY_A]) {
			move -= Vec3::Normalize(Vec3::Cross(cameraView, up));
		}
		if (m_window.m_keys[GLFW_KEY_S]) {
			move -= cameraView;
		}
		if (m_window.m_keys[GLFW_KEY_D]) {
			move += Vec3::Normalize(Vec3::Cross(cameraView, up));
		}

		m_camera->UpdateCamera(mouseMoveX, mouseMoveY, move);
	}
	void UpdateShadows(const std::list<std::shared_ptr<Terrain>>& t_terrains, const std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities, const Light& t_sun) {
		m_shadowMapMaster.Render(t_entities, t_terrains, t_sun, *m_camera);
	}
};