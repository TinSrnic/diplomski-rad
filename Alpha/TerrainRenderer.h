#pragma once
#include <vector>
#include "TerrainShader.h"
#include "Terrain.h"

/*Class used for rendering Terrain objects.*/
class TerrainRenderer {
private:
	TerrainShader& m_terrainShader;

public:
	TerrainRenderer(TerrainShader& shader) : m_terrainShader(shader) { }

	void Render(const std::list<std::shared_ptr<Terrain>>& t_terrains) {
		for each(std::shared_ptr<Terrain> terrain in t_terrains) {
			PrepareTerrain(terrain);
			PrepareModelMatrix(terrain);
			glDrawElements(GL_TRIANGLES, terrain->GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			UnbindTerrain();
		}
	}

	void PrepareTerrain(std::shared_ptr<Terrain> t_terrain) {
		TerrainTexturePack pack = t_terrain->GetTexturePack();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, pack.GetBackgroundTexture().GetTextureID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pack.GetRTexture().GetTextureID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, pack.GetGTexture().GetTextureID());
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, pack.GetBTexture().GetTextureID());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, t_terrain->GetBlendMap().GetTextureID());

		glBindVertexArray(t_terrain->GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		m_terrainShader.LoadShineVariables(0, 1);
	}
	void PrepareModelMatrix(std::shared_ptr<Terrain> t_terrain) {
		m_terrainShader.LoadModelMatrix(Mat4::Translation(Vec3(t_terrain->GetPosition().x, 0.0f, t_terrain->GetPosition().y)));
	}
	void UnbindTerrain() {
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
};