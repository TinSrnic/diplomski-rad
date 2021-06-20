#pragma once
#include <al.h>
#include <string>
#include "Vec3.h"

class Source {

private:
	ALuint m_sourceID = 0;
	ALuint m_currentBuffer;
	float m_gain;
	float m_pitch;
	Vec3 m_position;
	Vec3 m_velocity;
	bool m_looping;

public:
	Source(float gain, float pitch, Vec3 position, Vec3 velocity, bool looping) {
		m_gain = gain;
		m_pitch = pitch;
		m_position = position;
		m_velocity = velocity;
		m_looping = looping;

		alGenSources(1, &m_sourceID);

		alSourcef(m_sourceID, AL_PITCH, pitch);
		alSourcef(m_sourceID, AL_GAIN, gain);
		alSource3f(m_sourceID, AL_POSITION, position.x, position.y, position.z);
		alSource3f(m_sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
		if(looping) alSourcei(m_sourceID, AL_LOOPING, AL_TRUE);
		else alSourcei(m_sourceID, AL_LOOPING, AL_FALSE);
	}

	ALuint GetSourceID() { return m_sourceID; }

	void ChangeSound(ALuint buffer) {
		alSourceStop(m_sourceID);
		alSourcei(m_sourceID, AL_BUFFER, buffer);
	}
	void ChangeVolume(float volume) { 
		m_gain = volume;
		alSourcef(m_sourceID, AL_GAIN, volume); 
	}
	void ChangePitch(float pitch) { 
		m_pitch = pitch;
		alSourcef(m_sourceID, AL_PITCH, pitch); 
	}
	void ChangePosition(Vec3 position) { 
		m_position = position;
		alSource3f(m_sourceID, AL_POSITION, position.x, position.y, position.z); 
	}
	void ChangeVelocity(Vec3 velocity) { 
		m_velocity = velocity;
		alSource3f(m_sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z); 
	}
	void ChangeLooping(bool looping) { 
		m_looping = looping;
		if (looping) alSourcei(m_sourceID, AL_LOOPING, AL_TRUE);
		else alSourcei(m_sourceID, AL_LOOPING, AL_FALSE); 
	}

	void Play() { alSourcePlay(m_sourceID); }
	void PauseContinue() { IsPlaying() ? alSourcePause(m_sourceID) : alSourcePlay(m_sourceID); }
	bool IsPlaying() { 
		ALenum state;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}
	void Rewind() { 
		if (IsPlaying()) PauseContinue();
		alSourceRewind(m_sourceID); 
		PauseContinue();
	}
	void Stop() { alSourceStop(m_sourceID); }
};