#pragma once
#include <algorithm>
#include <iterator>
#include <al.h>
#include <alc.h>
#include <iostream>
#include <vector>
#include <map>
#include "Vec3.h"
#include "WavReader.h"
#include "Source.h"

class AudioMaster {

private:

	const unsigned int MAX_SFX = 20;
	ALCcontext* m_context;
	ALCdevice* m_device;

	std::vector<Source*> m_availableSFXSources;
	std::vector<Source*> m_unavailableSFXSources;
	std::vector<ALuint> m_sources;
	std::map<std::string, ALuint> m_buffers;

public:
	AudioMaster() {
		Init();
		for (unsigned int x = 0; x < MAX_SFX; x++) {
			m_availableSFXSources.push_back(new Source(1, 1, Vec3(0, 0, 0), Vec3(0, 0, 0), false));
		}
		AddBuffer("..\\Res\\Sound\\bounce.wav", "bounce");
		AddBuffer("..\\Res\\Sound\\main.wav", "main");
	}

	void Update() {
		std::vector<Source*>::iterator it = m_unavailableSFXSources.begin();
		while (it != m_unavailableSFXSources.end()) {
			ALenum sourceState;
			alGetSourcei((*it)->GetSourceID(), AL_SOURCE_STATE, &sourceState);
			if (sourceState != AL_PLAYING) {
				m_availableSFXSources.push_back(std::move(*it));
				it = m_unavailableSFXSources.erase(it);
			} else it++;
		}
	}

	Source* RequestSource() {
		if (m_availableSFXSources.empty()) return nullptr;
		std::vector<Source*>::iterator it = m_availableSFXSources.begin();

		m_unavailableSFXSources.push_back(std::move(*it));
		m_availableSFXSources.erase(it);

		Source* res = m_unavailableSFXSources.back();
		return res;
	}

	ALuint GetBuffer(std::string bufferName) { return m_buffers.at(bufferName); }

	Source AddSource() { 
		Source source = Source(1, 1, Vec3(0, 0, 0), Vec3(0, 0, 0), true);
		m_sources.push_back(source.GetSourceID()); 
		return source;
	}
	void AddBuffer(std::string filename, std::string soundName) {
		ALuint buffer;
		ALsizei size;
		ALsizei frequency;
		ALenum format;
		unsigned char* data = nullptr;

		loadWavFile(filename, data, size, frequency, format);

		alGenBuffers(1, &buffer);
		m_buffers.insert(std::pair<std::string, ALuint>(soundName, buffer));
		alBufferData(buffer, format, data, size, frequency);
		int error = alGetError();
	}

	void SetListenerData(Vec3 position, Vec3 velocity) {
		alListener3f(AL_POSITION, position.x, position.y, position.z);
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void CleanUp() {
		for each(ALuint source in m_sources) alDeleteSources(1, &source);
		for each(std::pair<std::string, ALuint> pair in m_buffers) alDeleteBuffers(1, &pair.second);
		m_device = alcGetContextsDevice(m_context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	unsigned int GetSFXCount() { return m_unavailableSFXSources.size(); }

private:
	void Init() {
		m_device = alcOpenDevice(nullptr);
		if (m_device == NULL) std::cerr << "Error finding default Audio Output Device" << std::endl;

		m_context = alcCreateContext(m_device, NULL);
		alcMakeContextCurrent(m_context);
	}
};