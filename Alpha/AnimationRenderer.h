#pragma once
#include "AnimationShader.h"
#include "AnimatedModel.h"

class AnimationRenderer {

private:
	AnimationShader& m_animationShader;

public:
	AnimationRenderer(AnimationShader& t_animationShader) : m_animationShader(t_animationShader) { }

	void Render(std::map<GLuint, std::vector<std::shared_ptr<AnimatedModel>>>& t_models) {
		for (std::map<GLuint, std::vector<std::shared_ptr<AnimatedModel>>>::iterator it = t_models.begin(); it != t_models.end(); ++it) {
			PrepareTexture(it->second.at(0));
			for each(std::shared_ptr<AnimatedModel> animatedModel in it->second) {
				PrepareInstance(animatedModel);
				glDrawElements(GL_TRIANGLES, animatedModel->GetIndicesCount(), GL_UNSIGNED_INT, (void*)0);
			}
			UnBindTexturedModel();
		}
	}

private:
	void PrepareTexture(std::shared_ptr<AnimatedModel> t_animatedModel) {
		glBindVertexArray(t_animatedModel->GetVaoID());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		m_animationShader.LoadNumberOfRows(1);
		m_animationShader.LoadShineVariables(0.01f, 1.0f);
		m_animationShader.LoadFakeLighting(false);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_animatedModel->GetTextureID());
	}
	void PrepareInstance(std::shared_ptr<AnimatedModel> t_animatedModel) {
		m_animationShader.LoadModelMatrix(t_animatedModel->GetModelMatrix());
		
		Bone* root = t_animatedModel->GetCurrentAnimation()->GetRootBone();
		m_animationShader.LoadJointMatrix(root->GetIndex(), root->GetCurrentTransform() * root->GetInverseStartTransform());

		RecursiveLoadBoneTransform(root);
	}
	void RecursiveLoadBoneTransform(Bone* t_root) {
		for each (Bone* child in t_root->GetChildren()) {
			m_animationShader.LoadJointMatrix(child->GetIndex(), child->GetCurrentTransform() * child->GetInverseStartTransform());
			Mat4 test = child->GetInverseStartTransform();
			RecursiveLoadBoneTransform(child);
		}
	}
	void UnBindTexturedModel() {
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

};