#pragma once
#include "Vec2i.h"
#include "RawModel.h"
#include "TerrainTexturePack.h"
#include "HeightGenerator.h"

/*Class used to represent a terrain, which is construted using a HeightGenerator.*/
class Terrain {
private:
	Vec2 m_position;

	RawModel m_rawModel;
	Texture m_blendMap;
	TerrainTexturePack m_texturePack;

	unsigned int m_vertexCount;
	float** m_heights;

public:
	Terrain(HeightGenerator& t_heightGenerator, Vec2i t_gridOffset, Texture t_blendMap, TerrainTexturePack t_texturePack, unsigned int t_vertexCount)
	: m_blendMap(t_blendMap), m_texturePack(t_texturePack), m_vertexCount(t_vertexCount), m_rawModel(GenerateTerrain(t_heightGenerator, t_gridOffset, t_vertexCount)) { }

	Terrain(HeightGenerator& t_heightGenerator, const std::string& t_filename, Vec2i t_gridOffset, Texture t_blendMap, TerrainTexturePack t_texturePack)
	: m_blendMap(t_blendMap), m_texturePack(t_texturePack), m_vertexCount(0), m_rawModel(GenerateFromHeightMap(t_heightGenerator, t_filename, t_gridOffset)) { }

	Vec2 GetPosition() { return m_position; }
	RawModel& GetRawModel() { return m_rawModel; }
	TerrainTexturePack& GetTexturePack() { return m_texturePack; }
	Texture& GetBlendMap() { return m_blendMap; }

	float GetHeight(const Vec2& worldPos) {
		float worldPosX = worldPos.x;
		float worldPosZ = worldPos.y;
		unsigned int terrainX = (unsigned int)(worldPosX - m_position.x);
		unsigned int terrainZ = (unsigned int)(worldPosZ - m_position.y);

		float gridSquareSize = TERRAIN_SIZE / (float)(m_vertexCount - 1);

		unsigned int gridX = (int)floor(terrainX / gridSquareSize);
		unsigned int gridZ = (int)floor(terrainZ / gridSquareSize);

		if (gridX >= m_vertexCount - 1 || gridZ >= m_vertexCount - 1 || gridX < 0 || gridZ < 0) return 0;

		float xCoord = (float)(fmod(terrainX, gridSquareSize) / gridSquareSize);
		float zCoord = (float)(fmod(terrainZ, gridSquareSize) / gridSquareSize);

		float answer;
		if (xCoord <= (1 - zCoord)) {
			answer = Math::BarryCentric(Vec3(0, m_heights[gridX][gridZ], 0),
				Vec3(1, m_heights[gridX + 1][gridZ], 0),
				Vec3(0, m_heights[gridX][gridZ + 1], 1),
				Vec2(xCoord, zCoord));
		} else {
			answer = Math::BarryCentric(Vec3(1, m_heights[gridX + 1][gridZ], 0),
				Vec3(1, m_heights[gridX + 1][gridZ + 1], 1),
				Vec3(0, m_heights[gridX][gridZ + 1], 1),
				Vec2(xCoord, zCoord));
		}

		return answer;
	}
	Vec3 PositionFromVector(const Vec3& t_cameraPosition, const Vec3& t_viewVector) {
		//binary search
		float maxDistance = 150.0f;
		float currentDistance = maxDistance;

		Vec3 currentTestPoint = t_cameraPosition + t_viewVector * currentDistance;
		if (currentTestPoint.y < GetHeight(Vec2(currentTestPoint.x, currentTestPoint.z))) {

		} else {

		}
	}

private:
	//TODO: optimize this
	RawModel GenerateTerrain(const HeightGenerator& t_heightGenerator, const Vec2i& t_gridOffset, unsigned int t_vertexCount) {
		m_vertexCount = t_vertexCount;
		m_position = Vec2((float)t_gridOffset.x * TERRAIN_SIZE, (float)t_gridOffset.y * TERRAIN_SIZE);
		m_heights = new float*[m_vertexCount];
		for (unsigned int x = 0; x < m_vertexCount; x++) m_heights[x] = new float[m_vertexCount];

		int count = m_vertexCount * m_vertexCount;

		float* vertices = new float[count * 3];
		float* textureCoords = new float[count * 2];
		float* normals = new float[count * 3];

		int vertexPointer = 0;
		for (unsigned int i = 0; i < m_vertexCount; i++) {
			for (unsigned int j = 0; j < m_vertexCount; j++) {
				float x = i / (float)(m_vertexCount - 1);
				float z = j / (float)(m_vertexCount - 1);
				
				m_heights[j][i] = t_heightGenerator.GenerateHeight(j + t_gridOffset.x * (int)(m_vertexCount - 1), i + t_gridOffset.y * (int)(m_vertexCount - 1));
				vertices[vertexPointer * 3 + 0] = z * TERRAIN_SIZE;
				vertices[vertexPointer * 3 + 1] = m_heights[j][i];
				vertices[vertexPointer * 3 + 2] = x * TERRAIN_SIZE;
				textureCoords[vertexPointer * 2] = z;
				textureCoords[vertexPointer * 2 + 1] = x;

				vertexPointer++;
			}
		}

		vertexPointer = 0;
		for (unsigned int x = 0; x < m_vertexCount; x++) {
			for (unsigned int y = 0; y < m_vertexCount; y++) {
				Vec3 normal = GenerateNormal(y, x, t_heightGenerator, t_gridOffset);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				vertexPointer++;
			}
		}
		
		unsigned int* indices = new unsigned int[6 * (m_vertexCount - 1) * (m_vertexCount - 1)];
		int pointer = 0;

		for (unsigned int gz = 0; gz < m_vertexCount - 1; gz++) {
			for (unsigned int gx = 0; gx < m_vertexCount - 1; gx++) {
				int topLeft = (gz * m_vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		return RawModel(vertices, count, textureCoords, normals, indices, 6 * (m_vertexCount - 1)*(m_vertexCount - 1), Vec3((float)TERRAIN_SIZE, 0.0f, (float)TERRAIN_SIZE));
	}

	RawModel GenerateFromHeightMap(const HeightGenerator& t_heightGenerator, const std::string& t_filename, const Vec2i& t_gridOffset) {
		unsigned int width, height;
		unsigned char* pixels;
		TextureLoader::GetTextureDimensions(t_filename, &height, &width);
		pixels = t_heightGenerator.GetHeightFromImage(t_filename);

		m_vertexCount = height;
		m_position = Vec2((float)t_gridOffset.x * TERRAIN_SIZE, (float)t_gridOffset.y * TERRAIN_SIZE);
		m_heights = new float*[m_vertexCount];
		for (unsigned int x = 0; x < m_vertexCount; x++) m_heights[x] = new float[m_vertexCount];

		int count = m_vertexCount * m_vertexCount;

		float* vertices = new float[count * 3];
		float* textureCoords = new float[count * 2];
		float* normals = new float[count * 3];

		int vertexPointer = 0;
		for (unsigned int i = 0; i < m_vertexCount; i++) {
			for (unsigned int j = 0; j < m_vertexCount; j++) {
				float x = i / (float)(m_vertexCount - 1);
				float z = j / (float)(m_vertexCount - 1);

				if (pixels == nullptr) m_heights[j][i] = 0;
				else m_heights[j][i] = (pixels[(i * m_vertexCount + j) * 4] / 255.0f - 0.5f) * 40.0f;

				vertices[vertexPointer * 3 + 0] = z * TERRAIN_SIZE;
				vertices[vertexPointer * 3 + 1] = m_heights[j][i];
				vertices[vertexPointer * 3 + 2] = x * TERRAIN_SIZE;
				textureCoords[vertexPointer * 2] = z;
				textureCoords[vertexPointer * 2 + 1] = x;

				vertexPointer++;
			}
		}

		vertexPointer = 0;
		for (unsigned int x = 0; x < m_vertexCount; x++) {
			for (unsigned int y = 0; y < m_vertexCount; y++) {
				Vec3 normal = GenerateNormal(y, x, t_heightGenerator, t_gridOffset);
				normals[vertexPointer * 3] = normal.x;
				normals[vertexPointer * 3 + 1] = normal.y;
				normals[vertexPointer * 3 + 2] = normal.z;
				vertexPointer++;
			}
		}

		unsigned int* indices = new unsigned int[6 * (m_vertexCount - 1) * (m_vertexCount - 1)];
		int pointer = 0;

		for (unsigned int gz = 0; gz < m_vertexCount - 1; gz++) {
			for (unsigned int gx = 0; gx < m_vertexCount - 1; gx++) {
				int topLeft = (gz * m_vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * m_vertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		return RawModel(vertices, count, textureCoords, normals, indices, 6 * (m_vertexCount - 1)*(m_vertexCount - 1), Vec3((float)TERRAIN_SIZE, 0.0f, (float)TERRAIN_SIZE));
	}

	//Generates a normal once the heights are generated, if the input is out of bounds it generates height
	Vec3 GenerateNormal(unsigned int x, unsigned int z, const HeightGenerator& t_heightGenerator, const Vec2i& t_gridOffset) {
		float heightL;
		float heightR;
		float heightD;
		float heightU;

		if (x == 0) heightL = t_heightGenerator.GenerateHeight(-1 + t_gridOffset.x * (int)(m_vertexCount - 1), z + t_gridOffset.y * (int)(m_vertexCount - 1));
		else heightL = m_heights[x - 1][z + 0];

		if (x == m_vertexCount - 1) heightR = t_heightGenerator.GenerateHeight(m_vertexCount + t_gridOffset.x * (int)(m_vertexCount - 1), z + t_gridOffset.y * (int)(m_vertexCount - 1));
		else heightR = m_heights[x + 1][z + 0];

		if (z == 0) heightD = t_heightGenerator.GenerateHeight(x + t_gridOffset.x * (int)(m_vertexCount - 1), -1 + t_gridOffset.y * (int)(m_vertexCount - 1));
		else heightD = m_heights[x + 0][z - 1];

		if (z == m_vertexCount - 1) heightU = t_heightGenerator.GenerateHeight(x + t_gridOffset.x * (int)(m_vertexCount - 1), m_vertexCount + t_gridOffset.y * (int)(m_vertexCount - 1));
		else heightU = m_heights[x + 0][z + 1];

		Vec3 normal = Vec3(heightL - heightR, 2.0f, heightD - heightU);
		normal = Vec3::Normalize(normal);
		return normal;
	}
};

