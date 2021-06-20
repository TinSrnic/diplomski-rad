#pragma once
#include "Math.h"
#include "Texture.h"
#include "WorldConstants.h"
#include "ParticleTexture.h"

/*Class used to represent a single particle*/
class Particle {

private:
	ParticleTexture& m_texture; //texture of the particle
	Vec3 m_position; //current position of the particle
	Vec3 m_velocity; //current speed of the particle
	float m_lifeLength; //the total lifeLenght of the particle
	float m_gravityEffect; //how much gravity effects the particle
	float m_scale; //how much the particle is scaled
	float m_distanceFromCamera; //so you can sort the particles

	float m_elapsedTime = 0; //time since particle creation

public:
	Particle(ParticleTexture& t_texture, const Vec3& t_position, const Vec3& t_speed, float t_lifeLength, float t_gravityEffect, float t_scale, const Vec3& t_cameraPosition)
	: m_texture(t_texture) {
		m_position = t_position;
		m_velocity = t_speed;
		m_lifeLength = t_lifeLength;
		m_gravityEffect = t_gravityEffect;
		m_scale = t_scale;
		m_distanceFromCamera = (t_cameraPosition - m_position).Length();
	}

	ParticleTexture& GetTexture() const { return m_texture; }
	Vec3 GetPosition() const { return m_position; }
	float GetScale() const { return m_scale; }

	bool Update(const Vec3& t_cameraPosition, float t_elapsedTime) {
		m_velocity.y += GRAVITY * t_elapsedTime * m_gravityEffect;
		m_position += m_velocity * t_elapsedTime;
		m_elapsedTime += t_elapsedTime;
		m_distanceFromCamera = (t_cameraPosition - m_position).Length();

		if (m_elapsedTime > m_lifeLength) return false;
		else return true;
	}

	/*Since the particles are interpolated between two images every frame,
	offset1 is the offest to the first image and offset2 is second, blend is how close to
	one or the other the particle is*/
	void GetOffsetAndBlend(Vec4& t_offset, float& t_blend) const {
		//TODO: optimize
		unsigned int numberOfRows = m_texture.GetNumberOfRows();
		float unit = m_lifeLength / (numberOfRows * numberOfRows);
		unsigned int squareNumber = (unsigned int)(m_elapsedTime / unit);

		unsigned int row1 = squareNumber / numberOfRows;
		unsigned int column1 = squareNumber % numberOfRows;

		unsigned int nextSquare;
		nextSquare = (squareNumber + 1 > numberOfRows * numberOfRows) ? squareNumber : squareNumber + 1;

		unsigned int row2 = nextSquare / numberOfRows;
		unsigned int column2 = nextSquare % numberOfRows;

		t_offset = Vec4((float)column1 / numberOfRows, (float)(numberOfRows - row1 - 1) / numberOfRows, (float)column2 / numberOfRows, (float)(numberOfRows - row2 - 1) / numberOfRows);
		t_blend = (float)(((squareNumber + 1) * unit - m_elapsedTime) / unit);
	}

	static bool CompareParticles(const Particle& t_first, const Particle& t_second) {
		return t_first.m_distanceFromCamera > t_second.m_distanceFromCamera;
	}
};