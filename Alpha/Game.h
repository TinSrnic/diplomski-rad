#pragma once
#include "Engine.h"
#include "GameObjects.h"

//Class for storing all the object which will then be rendered
class Game {

public:
	Engine& m_engine;
	bool m_collision;

public:
	Game(Engine& engine) : m_engine(engine) {
		/*m_audioMaster.AddBuffer("..\\Res\\Sound\\main.wav", "main");
		m_ambientMusic.ChangeSound(m_audioMaster.GetBuffer("main"));
		m_ambientMusic.ChangeVolume(0.1f);
		m_ambientMusic.Play();*/
		m_collision = false;
	}

	void PushToEngine() {
		m_engine.Render(GameObjects::m_terrains, GameObjects::m_skybox, GameObjects::m_entities,  GameObjects::m_lines, GameObjects::m_GUIs, GameObjects::m_texts, GameObjects::m_lights);
	}
	void UpdateWindow() {
		m_engine.UpdateWindow();
	}
	void EnableCollision() {
		m_collision = !m_collision;
	}
};