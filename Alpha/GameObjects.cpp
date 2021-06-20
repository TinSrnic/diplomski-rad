#include "GameObjects.h"
#include <algorithm>
#include "Entity.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Line.h"
#include "Light.h"
#include "Text.h"
#include "GUI.h"

#include "ModelLoader.h"
#include "TextureLoader.h"

std::vector<Light> GameObjects::m_lights = std::vector<Light>();
std::map<GLuint, std::list<std::shared_ptr<Entity>>> GameObjects::m_entities = std::map<GLuint, std::list<std::shared_ptr<Entity>>>();
std::list<std::shared_ptr<Terrain>> GameObjects::m_terrains = std::list<std::shared_ptr<Terrain>>();
std::map<GLuint, std::vector<std::shared_ptr<GUI>>> GameObjects::m_GUIs = std::map<GLuint, std::vector<std::shared_ptr<GUI>>>();
std::map<GLuint, std::vector<std::shared_ptr<Text>>> GameObjects::m_texts = std::map<GLuint, std::vector<std::shared_ptr<Text>>>();
std::shared_ptr<Skybox> GameObjects::m_skybox;
std::vector<std::shared_ptr<Line>> GameObjects::m_lines = std::vector<std::shared_ptr<Line>>();

void GameObjects::Add(const Light& light) {
	m_lights.push_back(light);
}
void GameObjects::Add(const std::shared_ptr<Entity>& t_entity) {
	try {
		m_entities[t_entity->GetTexture().GetTextureID()].push_back(t_entity);
	} catch (std::out_of_range) {
		m_entities.insert(std::pair<GLuint, std::list<std::shared_ptr<Entity>>>
			(t_entity->GetTexture().GetTextureID(), std::list<std::shared_ptr<Entity>>({ t_entity })));
	}
}
void GameObjects::Add(const std::shared_ptr<Terrain>& t_terrain) {
	m_terrains.push_back(t_terrain);
}
void GameObjects::Add(const std::shared_ptr<Skybox>& t_skybox) {
	m_skybox = t_skybox;
}
void GameObjects::Add(const std::shared_ptr<GUI>& t_gui) {
	GLuint textureID = t_gui->GetTexture().GetTextureID();
	try {
		m_GUIs[textureID].push_back(t_gui);
	}
	catch (std::out_of_range) {
		m_GUIs.insert(std::pair<GLuint, std::vector<std::shared_ptr<GUI>>>
			(textureID, std::vector<std::shared_ptr<GUI>>({ t_gui })));
	}

	std::sort(m_GUIs.at(textureID).begin(), m_GUIs.at(textureID).end(), [](const std::shared_ptr<GUI>& a, const std::shared_ptr<GUI>& b) -> bool {
		return a->GetPosition().z < b->GetPosition().z;
		});

	t_gui->AddedToGame();
}
void GameObjects::Add(const std::shared_ptr<Text>& t_text) {
	try {
		m_texts[t_text->GetFont().GetTextureID()].push_back(t_text);
	}
	catch (std::out_of_range) {
		m_texts.insert(std::pair<GLuint, std::vector<std::shared_ptr<Text>>>
			(t_text->GetFont().GetTextureID(), std::vector<std::shared_ptr<Text>>({ t_text })));
	}
}

void GameObjects::Add(const std::shared_ptr<Line>& t_line) {
	m_lines.push_back(t_line);
}

void GameObjects::ClearGUIs() {
	m_GUIs.clear();
}
void GameObjects::ClearEntities() {
	m_entities.clear();
}

void GameObjects::Remove(const std::shared_ptr<Line>& t_line) {
	m_lines.erase(std::remove(m_lines.begin(), m_lines.end(), t_line), m_lines.end());
}
void GameObjects::RemoveAllLines() {
	m_lines.clear();
}

void GameObjects::RemoveText() {
	m_texts.clear();
}