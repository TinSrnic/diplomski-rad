#pragma once
#include <glew.h>
#include "Window.h"
#include "WorldConstants.h"

/*FBO or frame buffer objects are what you render to. 
They can have attachments such as color or depth and by rendering to them you can use them as regular textures. 
Or you could use the depth data for some sort of calculation or something.*/
class WaterFBO {

private:
	GLuint reflectionFrameBuffer; //the buffer to which you render reflections has two attachments
	GLuint reflectionTexture; //the color attachment to which you render reflections
	GLuint reflectionDepthBuffer; //TODO: figure out what this does

	GLuint refractionFrameBuffer; //the buffer to which you render refractions has two attachments
	GLuint refractionTexture; //the color attachment to which you render refraction scene
	GLuint refractionDepthTexture; //the depth attachment to which you render refraction scene depth

public:
	WaterFBO(const Window& t_window) {
		InitialiseReflectionFrameBuffer(t_window);
		InitialiseRefractionFrameBuffer(t_window);
	}

	void BindReflectionFrameBuffer() const { BindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT); }
	void BindRefractionFrameBuffer() const { BindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT); }

	GLuint GetReflectionTexture() const { return reflectionTexture; }
	GLuint GetRefractionTexture() const { return refractionTexture; }
	GLuint GetRefractionDepthTexture() const { return refractionDepthTexture; }

	void BindFrameBuffer(GLuint t_frameBuffer, unsigned int t_width, unsigned int t_height) const {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, t_frameBuffer);
		glViewport(0, 0, t_width, t_height);
	}
	void UnbindCurrentFrameBuffer(const Window& t_window) const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, t_window.m_width, t_window.m_height);
	}
	void CleanUp() const {
		glDeleteFramebuffers(1, &reflectionFrameBuffer);
		glDeleteTextures(1, &reflectionTexture);
		glDeleteRenderbuffers(1, &reflectionDepthBuffer);
		glDeleteFramebuffers(1, &refractionFrameBuffer);
		glDeleteTextures(1, &refractionTexture);
		glDeleteTextures(1, &refractionDepthTexture);
	}

private:
	void InitialiseReflectionFrameBuffer(const Window& window) {
		reflectionFrameBuffer = CreateFrameBuffer();
		reflectionTexture = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		reflectionDepthBuffer = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		UnbindCurrentFrameBuffer(window);
	}
	void InitialiseRefractionFrameBuffer(const Window& window) {
		refractionFrameBuffer = CreateFrameBuffer();
		refractionTexture = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		refractionDepthTexture = CreateDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		UnbindCurrentFrameBuffer(window);
	}

private:
	GLuint CreateFrameBuffer() const  {
		GLuint frameBufferID;
		glGenFramebuffers(1, &frameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		return frameBufferID;
	}
	GLuint CreateTextureAttachment(unsigned int t_width, unsigned int t_height) const  {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureID, 0);
		return textureID;
	}
	GLuint CreateDepthTextureAttachment(unsigned int t_width, unsigned int t_height) const  {
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, t_width, t_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureID, 0);
		return textureID;
	}
	GLuint CreateDepthBufferAttachment(unsigned int t_width, unsigned int t_height) const  {
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t_width, t_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		return depthBuffer;
	}
};