#pragma once
#include "Particle.h"
#include "Math.h"

class ParticleEmitter {

private:
	ParticleTexture m_texture;
	Vec3 m_position;
	Vec3 m_velocity;
	float m_lifeLength;
	float m_gravityEffect;
	float m_scale;

	Vec3 m_positionErrorMin = Vec3(0.0f);
	Vec3 m_positionErrorMax = Vec3(0.0f);

	Vec3 m_velocityErrorMin = Vec3(0.0f);
	Vec3 m_velocityErrorMax = Vec3(0.0f);

	//vec2 for min and max offset
	Vec2 m_lifeLengthError = Vec2(0.0f, 0.0f);
	Vec2 m_gravityEffectError = Vec2(0.0f, 0.0f);
	Vec2 m_scaleError = Vec2(0.0f, 0.0f);

	float m_particlesPerSecond;
	std::list<Particle> m_particles;
	float m_totalElapsedTime = 0;

public:
	ParticleEmitter(ParticleTexture& t_texture, float t_particlesPerSecond, const Vec3& t_position, const Vec3& t_velocity, float t_gravityEffect, float t_lifeLength, float t_scale) 
	: m_texture(t_texture) {
		m_particlesPerSecond = t_particlesPerSecond;
		m_position = t_position;
		m_velocity = t_velocity;
		m_gravityEffect = t_gravityEffect;
		m_lifeLength = t_lifeLength;
		m_scale = t_scale;
	}

	ParticleTexture& GetTexture() { return m_texture; }

	Vec3 GetPosition() { return m_position; }
	Vec3 GetVelocity() { return m_velocity; }
	float GetLifeLength() { return m_lifeLength; }
	float GetGravityEffect() { return m_gravityEffect; }
	float GetScale() { return m_scale; }

	Vec3 GetPositionErrorMin() { return m_positionErrorMin; }
	Vec3 GetPositionErrorMax() { return m_positionErrorMin; }
	Vec3 GetVelocityErrorMin() { return m_velocityErrorMin; }
	Vec3 GetVelocityErrorMax() { return m_velocityErrorMax; }
	Vec2 GetLifeLengthError() { return m_lifeLengthError; }
	Vec2 GetGravityEffectError() { return m_gravityEffectError; }
	Vec2 GetScaleError() { return m_scaleError; }

	std::list<Particle>& GetParticles() { return m_particles; }

	void SetPosition(const Vec3& t_position) { m_position = t_position; }
	void SetVelocity(const Vec3& t_velocity) { m_velocity = t_velocity; }
	void SetScale(const float t_scale) { m_scale = t_scale; }
	void SetParticlesPerSecond(const float t_particlesPerSecond) { m_particlesPerSecond = t_particlesPerSecond; }

	void SetPositionErrorMin(const Vec3& t_positionErrorMin) { m_positionErrorMin = t_positionErrorMin; }
	void SetPositionErrorMax(const Vec3& t_positionErrorMax) { m_positionErrorMax = t_positionErrorMax; }
	void SetVelocityErrorMin(const Vec3& t_velocityErrorMin) { m_velocityErrorMin = t_velocityErrorMin; }
	void SetVelocityErrorMax(const Vec3& t_velocityErrorMax) { m_velocityErrorMax = t_velocityErrorMax; }
	void SetLifeLengthError(const Vec2& t_lifeError) { m_lifeLengthError = t_lifeError; }
	void SetGravityEffectError(const Vec2& t_gravityError) { m_gravityEffectError = t_gravityError; }
	void SetScaleError(const Vec2& t_scaleError) { m_scaleError = t_scaleError; }

	void Update(const Vec3& t_cameraPosition, float t_elapsedTime) {
		std::list<Particle>::iterator it = m_particles.begin();
		while(it != m_particles.end()) {
			if (!it->Update(t_cameraPosition, t_elapsedTime)) {
				it = m_particles.erase(it);
			} else {
				++it;
			}
		}

		//TODO: optimize this
		m_totalElapsedTime += t_elapsedTime;
		unsigned int particlesToCreate = 0;
		float unit = 1.0f / m_particlesPerSecond;

		while (m_totalElapsedTime >= unit) {
			m_totalElapsedTime -= unit;
			particlesToCreate++;
		}

		for (unsigned int x = 0; x < particlesToCreate; x++) {
			Vec3 randomPos = Vec3(Math::RandomFloat(m_positionErrorMin.x, m_positionErrorMax.x), Math::RandomFloat(m_positionErrorMin.y, m_positionErrorMax.y), Math::RandomFloat(m_positionErrorMin.z, m_positionErrorMax.z));
			Vec3 randomVel = Vec3(Math::RandomFloat(m_velocityErrorMin.x, m_velocityErrorMax.x), Math::RandomFloat(m_velocityErrorMin.y, m_velocityErrorMax.y), Math::RandomFloat(m_velocityErrorMin.z, m_velocityErrorMax.z));
			float randomLife = Math::RandomFloat(m_lifeLengthError.x, m_lifeLengthError.y);
			float randomGravity = Math::RandomFloat(m_gravityEffectError.x, m_gravityEffectError.y);
			float randomScale = Math::RandomFloat(m_scaleError.x, m_scaleError.y);
			m_particles.push_back(Particle(m_texture, m_position + randomPos, m_velocity + randomVel, m_lifeLength + randomLife, m_gravityEffect + randomGravity, m_scale + randomScale, t_cameraPosition));
		}

		m_particles.sort(Particle::CompareParticles);
	}
};