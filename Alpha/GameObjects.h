#pragma once
#include <map>
#include <list>
#include <vector>
#include <memory>
#include <sstream>
#include <glew.h>
#include "Math.h"

class Entity; 
class Terrain;
class GUI;
class Text;
class Skybox;
class Line;

class Light;

class GameObjects {
	
	friend class Game;
private:
	static std::vector<Light> m_lights;
	 
	//all objects
	static std::map<GLuint, std::list<std::shared_ptr<Entity>>> m_entities;
	static std::list<std::shared_ptr<Terrain>> m_terrains;
	static std::map<GLuint, std::vector<std::shared_ptr<GUI>>> m_GUIs;
	static std::map<GLuint, std::vector<std::shared_ptr<Text>>> m_texts;
	static std::shared_ptr<Skybox> m_skybox;
	static std::vector<std::shared_ptr<Line>> m_lines;

public:
	static void Add(const Light& light);
	static void Add(const std::shared_ptr<Entity>& t_entity);
	static void Add(const std::shared_ptr<Terrain>& t_terrain);
	static void Add(const std::shared_ptr<GUI>& t_gui);
	static void Add(const std::shared_ptr<Text>& t_text);
	static void Add(const std::shared_ptr<Skybox>& t_skybox);
	static void Add(const std::shared_ptr<Line>& t_line);

	static void ClearGUIs();
	static void ClearEntities();
	static void Remove(const std::shared_ptr<Line>& t_line);
	static void RemoveAllLines();

	static void RemoveText();
};