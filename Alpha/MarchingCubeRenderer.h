#pragma once
#include <vector>
#include "Mat4.h"
#include "MarchingCubeShader.h"
#include "MarchingCubeVAO.h"
#include "WorldConstants.h"
#include "Light.h"

/*Class used for rendering Terrain objects.*/
class MarchingCubeRenderer {
private:
	MarchingCubeShader& m_marchingCubeShader = MarchingCubeShader();
	float m_dimLightsFactor = 1.0f;

public:
	MarchingCubeRenderer(Mat4 persepectiveMatrix) { 
		Vec3 FOG_COLOR = Vec3(0.5f, 0.5f, 0.5f);

		m_marchingCubeShader.BindShader();
		m_marchingCubeShader.LoadProjectionMatrix(persepectiveMatrix);
		m_marchingCubeShader.LoadSkyColor(FOG_COLOR);
		m_marchingCubeShader.LoadDrawDistance(DRAW_DISTANCE);
		m_marchingCubeShader.UnbindShader();
	}

	void Render(std::vector<MarchingCubeVAO>& cubeTerrain, Mat4 viewMatrix, const std::vector<Light>& t_lights) {			
		m_marchingCubeShader.BindShader();
		glDisable(GL_CULL_FACE);

		m_marchingCubeShader.LoadTerrainScale(MARCHING_CUBE_TERRAIN_SCALE);

		std::vector<Light> updatedLights;
		for each (Light light in t_lights) updatedLights.push_back(Light(light.GetPosition(), light.GetColor() * m_dimLightsFactor, light.GetAttenuation()));
		m_marchingCubeShader.LoadLights(updatedLights);
		m_marchingCubeShader.LoadShineVariables(0.1, 1);

		m_marchingCubeShader.LoadViewMatrix(viewMatrix);

		for each (MarchingCubeVAO vao in cubeTerrain) {
			glBindVertexArray(vao.GetVaoID());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			m_marchingCubeShader.LoadModelMatrix(vao.GetModelMatrix());
			//glDrawElements(GL_TRIANGLES, vao.GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			glDrawArrays(GL_TRIANGLES, 0, vao.GetVertexCount());

			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		glEnable(GL_CULL_FACE);
		m_marchingCubeShader.UnbindShader();
	}
};