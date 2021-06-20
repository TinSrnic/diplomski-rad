#include "Window.h"

/*
 * The frame buffer for the shadow pass. This class sets up the depth texture
 * which can be rendered to during the shadow render pass, producing a shadow
 * map.
 */
class ShadowFrameBuffer {

private:
	GLuint m_FBO;
	GLuint m_shadowMap;

public:
	ShadowFrameBuffer(Window& t_window) {
		m_FBO = CreateFrameBuffer();
		m_shadowMap = CreateDepthBufferAttachment(SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
		UnbindFrameBuffer(t_window);
	}

	int GetShadowMap() { return m_shadowMap; }

	/*
	* Binds the frame buffer as the current render target.
	*
	* @param frameBuffer
	*            - the frame buffer.
	* @param width
	*            - the width of the frame buffer.
	* @param height
	*            - the height of the frame buffer.
	*/
	void BindFrameBuffer() {
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, SHADOW_MAP_WIDTH, SHADOW_MAP_HEIGHT);
	}
	/*
	* Unbinds the frame buffer, setting the default frame buffer as the current
	* render target.
	*/
	void UnbindFrameBuffer(const Window& t_window) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, t_window.GetWidth(), t_window.GetHeight());
	}
	/*
	 * Deletes the frame buffer and shadow map texture when the game closes.
	 */
	void CleanUp() {
		glDeleteFramebuffers(1, &m_FBO);
		glDeleteTextures(1, &m_shadowMap);
	}

private:
	/*
	 * Creates a frame buffer and binds it so that attachments can be added to
	 * it. The draw buffer is set to none, indicating that there's no color
	 * buffer to be rendered to.
	 * 
	 * @return The newly created frame buffer's ID.
	 */
	int CreateFrameBuffer() {
		GLuint frameBuffer;
		glGenFramebuffers(1, &frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		return frameBuffer;
	}
	/*
	 * Creates a depth buffer texture attachment.
	 * 
	 * @param width
	 *            - the width of the texture.
	 * @param height
	 *            - the height of the texture.
	 * @return The ID of the depth texture.
	 */
	GLuint CreateDepthBufferAttachment(unsigned int t_width, unsigned int t_height) {
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, t_width, t_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void*)0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
		return texture;
	}
};
