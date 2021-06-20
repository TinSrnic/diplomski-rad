#include "ModelLoader.h"
#include <fstream>
#include "StringManipulation.h"
#include "RawModel.h"
#include "Math.h"
#include "WorldConstants.h"

std::map<std::string, RawModel> ModelLoader::m_models = std::map<std::string, RawModel>();

RawModel& ModelLoader::LoadModel(const std::string& t_filepath) {
	try {
		return m_models.at(t_filepath);
	} catch (std::out_of_range ex) {
		return m_models.insert(std::pair<std::string, RawModel>(t_filepath, LoadOBJ(t_filepath))).first->second;
	}
}
std::map<std::string, RawModel>& ModelLoader::GetModels() { return m_models; }

void ModelLoader::ProcessVertex(const std::string& t_file, std::vector<std::string>& vertexData, std::vector<GLuint>* indices, std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare>* map, GLuint& currentIndCount) {
	if (vertexData.size() < 3) return;
	Vec3i data = Vec3i(std::stoi(vertexData[0]) - 1, std::stoi(vertexData[1]) - 1, std::stoi(vertexData[2]) - 1);
	if (map->find(data) != map->end()) {
		indices->push_back(map->at(data));
	} else {
		map->insert(std::pair<Vec3i, GLuint>(data, currentIndCount));
		indices->push_back(currentIndCount);
		currentIndCount++;
	}
}
RawModel ModelLoader::LoadOBJ(const std::string& t_filePath) {
	std::ifstream infile(MODEL_PATH + t_filePath);

	std::map<Vec3i, GLuint, Vec3i::Vec3iMapCompare> map;
	std::vector<Vec3*> vertices;
	std::vector<Vec2*> textures;
	std::vector<Vec3*> normals;
	std::vector<GLuint> indices;

	GLfloat *verticesArray = nullptr;
	GLfloat *texturesArray = nullptr;
	GLfloat *normalsArray = nullptr;
	GLuint *indicesArray = nullptr;

	float width = 0.0f;
	float minX = 100000.0f;
	float maxX = -100000.0f;

	float height = 0.0f;
	float minY = 100000.0f;
	float maxY = -100000.0f;

	float length = 0.0f;
	float minZ = 100000.0f;
	float maxZ = -100000.0f;

	std::string line;
	unsigned int currentIndCount = 0;
	while (std::getline(infile, line)) {
		std::vector<std::string> currentLine = StringManipulation::SplitString(line, ' ');
		if (line[0] == 'v' && line[1] == ' ') {
			Vec3* vertex = new Vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
			vertices.push_back(vertex);
			if (vertex->x < minX) minX = vertex->x;
			else if (vertex->x > maxX) maxX = vertex->x;
			if (vertex->y < minY) minY = vertex->y;
			else if (vertex->y > maxY) maxY = vertex->y;
			if (vertex->z < minZ) minZ = vertex->z;
			else if (vertex->z > maxZ) maxZ = vertex->z;
		} else if (line[0] == 'v' && line[1] == 't') {
			Vec2* texture = new Vec2(std::stof(currentLine[1]), std::stof(currentLine[2]));
			textures.push_back(texture);
		} else if (line[0] == 'v' && line[1] == 'n') {
			Vec3* normal = new Vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
			normals.push_back(normal);
		} else if (line[0] == 'f' && line[1] == ' ') {
			std::vector<std::string> face = StringManipulation::SplitString(line, ' ');
			std::vector<std::string> vertex1 = StringManipulation::SplitString(face[1], '/');
			std::vector<std::string> vertex2 = StringManipulation::SplitString(face[2], '/');
			std::vector<std::string> vertex3 = StringManipulation::SplitString(face[3], '/');

			ProcessVertex(t_filePath, vertex1, &indices, &map, currentIndCount);
			ProcessVertex(t_filePath, vertex2, &indices, &map, currentIndCount);
			ProcessVertex(t_filePath, vertex3, &indices, &map, currentIndCount);
		}
	}

	//TODO: replace these and look at this function in general, you can probably speed it up
	//indicesArray = new GLuint[indices.size()];
	indicesArray = (GLuint*)malloc(sizeof(GLuint) * indices.size());
	verticesArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 3);
	texturesArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 2);
	normalsArray = (GLfloat*)malloc(sizeof(GLfloat) * map.size() * 3);

	for each(std::pair<Vec3i, GLuint> pair in map) {
		verticesArray[pair.second * 3 + 0] = vertices.at(pair.first.x)->x;
		verticesArray[pair.second * 3 + 1] = vertices.at(pair.first.x)->y;
		verticesArray[pair.second * 3 + 2] = vertices.at(pair.first.x)->z;

		texturesArray[pair.second * 2 + 0] = textures.at(pair.first.y)->x;
		texturesArray[pair.second * 2 + 1] = textures.at(pair.first.y)->y;

		normalsArray[pair.second * 3 + 0] = normals.at(pair.first.z)->x;
		normalsArray[pair.second * 3 + 1] = normals.at(pair.first.z)->y;
		normalsArray[pair.second * 3 + 2] = normals.at(pair.first.z)->z;
	}

	for (unsigned int x = 0; x < indices.size(); x++) indicesArray[x] = indices.at(x);

	width = maxX - minX;
	height = maxY - minY;
	length = maxZ - minZ;

	return RawModel(verticesArray, map.size(), texturesArray, normalsArray, indicesArray, (GLuint)indices.size(), Vec3(width, height, length));
}