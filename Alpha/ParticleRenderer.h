#pragma once
#include <map>
#include <vector>
#include "Camera.h"
#include "ParticleShader.h"
#include "ParticleEmitter.h"

class ParticleRenderer {

private:
	ParticleShader& m_particleShader;
	GLuint m_vaoID; // all particles use the same rawModel, so it's stored here
	GLuint m_vboID; // vbo used for all instanced rendering
	std::vector<GLfloat> m_vboData = std::vector<GLfloat>();
	unsigned int m_maxParticles = 10000;
	unsigned int m_bytesPerParticle = 21; //21 bytes = 16 for model matrix, 4 for offsets, 1 for blend factor

public:
	ParticleRenderer(ParticleShader& t_particleShader) : m_particleShader(t_particleShader) {
		const GLfloat vertices[8] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f };
		GLuint vertBufferID;

		glGenVertexArrays(1, &m_vaoID);
		glBindVertexArray(m_vaoID);

		//position buffer
		glGenBuffers(1, &vertBufferID);

		glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//instanced buffer
		glGenBuffers(1, &m_vboID);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_maxParticles * m_bytesPerParticle, 0, GL_STREAM_DRAW); //stream draw means the data changes frequently, which it does
		
		//REMEMBER, stride is when the rendering is done how many bytes it has to go ahead and offset is where that element is located in the current part of the vbo
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(0 * 4 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(1 * 4 * sizeof(GLfloat)));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(2 * 4 * sizeof(GLfloat)));
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(3 * 4 * sizeof(GLfloat)));
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(4 * 4 * sizeof(GLfloat)));
		glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * m_bytesPerParticle, (void*)(5 * 4 * sizeof(GLfloat)));
		
		glVertexAttribDivisor(1, 1); //the second number is how many instances it has to render before it increments
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Render(const std::map<GLuint, std::list<std::shared_ptr<ParticleEmitter>>>& t_particleEmitters, const Camera& t_camera) {
		Prepare(t_camera);
		


		for each(std::pair<GLuint, std::list<std::shared_ptr<ParticleEmitter>>> pair in t_particleEmitters) {
			m_vboData.clear(); //one cycle of vbo filling should fill it up with data of all the particles which use the same texture
			PrepareTexture(pair.second.begin()->get()->GetTexture());

			for each(std::shared_ptr<ParticleEmitter> particleEmitter in pair.second) {
				for each(Particle particle in particleEmitter->GetParticles()) {
					Mat4 modelMatrix = Mat4::Translation(particle.GetPosition()) * Mat4::Scale(Vec3(particle.GetScale()));
					for (unsigned int x = 0; x < 16; x++) {
						m_vboData.push_back(modelMatrix[x]);
					}

					Vec4 offset;
					float blend;
					particle.GetOffsetAndBlend(offset, blend);
					m_vboData.push_back(offset.x);
					m_vboData.push_back(offset.y);
					m_vboData.push_back(offset.z);
					m_vboData.push_back(offset.w);
					m_vboData.push_back(blend);

					//PrepareParticle(particle);
				}
			}

			if (m_vboData.size() != 0) {
				glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
				glBufferSubData(GL_ARRAY_BUFFER, 0, m_vboData.size() * sizeof(GLfloat), &m_vboData[0]);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_vboData.size() / m_bytesPerParticle);
			}
		}

		Unbind();
	}

private:
	void Prepare(const Camera& t_camera) {
		glBindVertexArray(m_vaoID);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);

		glEnable(GL_BLEND);
		glDepthMask(false); //don't render to the depth buffer

		m_particleShader.LoadViewMatrix(t_camera.GetViewMatrix());
		m_particleShader.LoadInverseRotationViewMatrix(Mat4::Invert(Mat4::RotationFromDirection(t_camera.GetViewDirection()))); //TODO: test if this is faster or if it is faster to manually set the matrix to the desired value
	}
	void PrepareTexture(const ParticleTexture& t_texture) {
		m_particleShader.LoadAtlasRows(t_texture.GetNumberOfRows());

		//TODO: look into what these functions do
		if (t_texture.GetUseAddative()) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE); //use addative blending so that the order in which particles are rendered doesn't matter
		} else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //use addative blending so that the order in which particles are rendered doesn't matter
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t_texture.GetTextureID());
	}
	void Unbind() {
		glDepthMask(true);
		glDisable(GL_BLEND);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glBindVertexArray(0);
	}
};