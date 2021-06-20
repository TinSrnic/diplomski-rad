#pragma once
#include "EntityShader.h"
#include "Entity.h"

/*Class used for rendering Entity objects. 
It groups them in a map whose key is the texture index, 
so that it saves time by binding the texture only as many times as neccessary.*/
class EntityRenderer {

private:
	EntityShader& m_entityShader;

public:
	EntityRenderer(EntityShader& t_entityShader) : m_entityShader(t_entityShader) { }

	void Render(const std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for each(std::pair<GLuint, std::list<std::shared_ptr<Entity>>> pair in t_entities) {
			PrepareTexturedModel(*(pair.second.begin()));
			for each(std::shared_ptr<Entity> entity in pair.second) {
				PrepareEntity(entity);
				glDrawElements(GL_TRIANGLES, entity->GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			}
			UnbindTexturedModel();
		}
		glDisable(GL_BLEND);
	}

private:
	void PrepareTexturedModel(std::shared_ptr<Entity> t_entity) {
		glBindVertexArray(t_entity->GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		m_entityShader.LoadNumberOfRows(t_entity->GetTexture().GetNumberOfRows());
		m_entityShader.LoadShineVariables(t_entity->GetTexture().GetReflectivity(), t_entity->GetTexture().GetShineDampening());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_entity->GetTexture().GetTextureID());
	}
	void PrepareEntity(std::shared_ptr<Entity> t_entity) {
		glBindVertexArray(t_entity->GetRawModel().GetVaoID());

		m_entityShader.LoadModelMatrix(t_entity->GetModelMatrix());
		m_entityShader.LoadFakeLighting(t_entity->GetFakeLighting());
		m_entityShader.LoadAtlasOffset(t_entity->GetAtlasOffset());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, t_entity->GetNormalMapID());
	}
	void UnbindTexturedModel() {
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
	
};