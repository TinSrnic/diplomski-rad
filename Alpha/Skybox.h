#pragma once
#include <vector>
#include "WorldConstants.h"
#include "TextureLoader.h"

/*Class used to represent a skybox*/
class Skybox {

private:
	GLuint m_vaoID;

	GLuint m_cubeMapSunriseID;
	GLuint m_cubeMapDayID;
	GLuint m_cubeMapSunsetID;
	GLuint m_cubeMapNightID;

	float m_rotationSpeed;

public:
	Skybox(std::string t_folderPathSunrise, std::string t_folderPathDay, std::string t_folderPathSunset, std::string t_floderPathNight, float t_rotationSpeed = 0.01f) {
		m_rotationSpeed = t_rotationSpeed;

		std::vector<std::string> filesSunrise;
		filesSunrise.push_back(t_folderPathSunrise + "\\right.png");
		filesSunrise.push_back(t_folderPathSunrise + "\\left.png");
		filesSunrise.push_back(t_folderPathSunrise + "\\top.png");
		filesSunrise.push_back(t_folderPathSunrise + "\\bottom.png");
		filesSunrise.push_back(t_folderPathSunrise + "\\back.png");
		filesSunrise.push_back(t_folderPathSunrise + "\\front.png");

		std::vector<std::string> filesDay;
		filesDay.push_back(t_folderPathDay + "\\right.png");
		filesDay.push_back(t_folderPathDay + "\\left.png");
		filesDay.push_back(t_folderPathDay + "\\top.png");
		filesDay.push_back(t_folderPathDay + "\\bottom.png");
		filesDay.push_back(t_folderPathDay + "\\back.png");
		filesDay.push_back(t_folderPathDay + "\\front.png");

		std::vector<std::string> filesSunset;
		filesSunset.push_back(t_folderPathSunset + "\\right.png");
		filesSunset.push_back(t_folderPathSunset + "\\left.png");
		filesSunset.push_back(t_folderPathSunset + "\\top.png");
		filesSunset.push_back(t_folderPathSunset + "\\bottom.png");
		filesSunset.push_back(t_folderPathSunset + "\\back.png");
		filesSunset.push_back(t_folderPathSunset + "\\front.png");

		std::vector<std::string> filesNight;
		filesNight.push_back(t_floderPathNight + "\\right.png");
		filesNight.push_back(t_floderPathNight + "\\left.png");
		filesNight.push_back(t_floderPathNight + "\\top.png");
		filesNight.push_back(t_floderPathNight + "\\bottom.png");
		filesNight.push_back(t_floderPathNight + "\\back.png");
		filesNight.push_back(t_floderPathNight + "\\front.png");

		m_cubeMapSunriseID = TextureLoader::LoadCubeTexture(filesSunrise);
		m_cubeMapDayID = TextureLoader::LoadCubeTexture(filesDay);
		m_cubeMapSunsetID = TextureLoader::LoadCubeTexture(filesSunset);
		m_cubeMapNightID = TextureLoader::LoadCubeTexture(filesNight);

		CreateVAO();
	}

	GLuint GetVaoID() const { return m_vaoID; }

	GLuint GetCubeMapSunriseID() const { return m_cubeMapSunriseID; }
	GLuint GetCubeMapDayID() const { return m_cubeMapDayID; }
	GLuint GetCubeMapSunsetID() const { return m_cubeMapSunsetID; }
	GLuint GetCubeMapNightID() const { return m_cubeMapNightID; }

	float GetRotationSpeed() const { return m_rotationSpeed; }

	void SetRotationSpeed(float t_rotationSpeed) { m_rotationSpeed = t_rotationSpeed; }
private:
	void CreateVAO() {
		GLfloat m_verts[108] = {
			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,

			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,

			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,

			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,

			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE,  SKYBOX_SIZE,
			-SKYBOX_SIZE,  SKYBOX_SIZE, -SKYBOX_SIZE,

			-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
			-SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE,
			SKYBOX_SIZE, -SKYBOX_SIZE,  SKYBOX_SIZE
		};

		GLuint vertBuffer;
		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		glGenBuffers(1, &vertBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, m_verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}
};