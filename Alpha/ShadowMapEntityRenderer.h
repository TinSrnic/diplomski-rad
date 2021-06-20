#pragma once
#include <map>
#include <list>
#include "Entity.h"
#include "ShadowShader.h"

class ShadowMapEntityRenderer {

private:
	ShadowShader& m_shadowShader;

public:
	/**
	* @param shader
	*            - the simple shader program being used for the shadow render
	*            pass.
	* @param projectionViewMatrix
	*            - the orthographic projection matrix multiplied by the light's
	*            "view" matrix.
	*/
	ShadowMapEntityRenderer(ShadowShader& t_shadowShader) : m_shadowShader(t_shadowShader) { }

	/**
	* Renders entieis to the shadow map. Each model is first bound and then all
	* of the entities using that model are rendered to the shadow map.
	*
	* @param entities
	*            - the entities to be rendered to the shadow map.
	*/
	void Render(const std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities, const std::list<std::shared_ptr<Terrain>>& t_terrains) {

		for each(std::shared_ptr<Terrain> terrain in t_terrains) {
			PrepareTerrain(terrain);
			glDrawElements(GL_TRIANGLES, terrain->GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
		}

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		for each (std::pair<GLuint, std::list<std::shared_ptr<Entity>>> pair in t_entities) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, pair.first);
			for each(std::shared_ptr<Entity> entity in pair.second) {
				PrepareEntity(entity);
				glDrawElements(GL_TRIANGLES, entity->GetRawModel().GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			}
		}

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		
	}

private:
	/*
	* Prepares an entity to be rendered. The model matrix is created in the
	* usual way and then multiplied with the projection and view matrix (often
	* in the past we've done this in the vertex shader) to create the
	* mvp-matrix. This is then loaded to the vertex shader as a uniform.
	*
	* @param entity
	*            - the entity to be prepared for rendering.
	*/
	void PrepareEntity(std::shared_ptr<Entity> t_entity) {
		glBindVertexArray(t_entity->GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		m_shadowShader.LoadModelMatrix(t_entity->GetModelMatrix());
	}

	void PrepareTerrain(std::shared_ptr<Terrain> t_terrain) {
		glBindVertexArray(t_terrain->GetRawModel().GetVaoID());
		glEnableVertexAttribArray(0);
		m_shadowShader.LoadModelMatrix(Mat4::Translation(Vec3(t_terrain->GetPosition().x, 0.0f, t_terrain->GetPosition().y)));
	}
};