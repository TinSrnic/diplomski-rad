#pragma once
#include <map>
#include <vector>
#include <glew.h>
#include "Vec3i.h"

class RawModel;

/*
Class used to manage all loaded models can be used for massive speed up
*/
class ModelLoader {

private:
	static std::map<std::string, RawModel> m_models;

public:
	static RawModel& LoadModel(const std::string& t_filePath);
	static std::map<std::string, RawModel>& GetModels();
	
private:
	static void ProcessVertex(const std::string& t_file, std::vector<std::string>& vertexData, std::vector<GLuint>* indices, std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare>* map, GLuint& currentIndCount);
	static RawModel LoadOBJ(const std::string& t_filePath);
};