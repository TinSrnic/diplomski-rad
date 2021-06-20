#include "ShadowMapEntityRenderer.h"
#include "ShadowFrameBuffer.h"
#include "ShadowBox.h"
#include "Camera.h"
#include "Light.h"

/*
 * This class is in charge of using all of the classes in the shadows package to
 * carry out the shadow render pass, i.e. rendering the scene to the shadow map
 * texture. This is the only class in the shadows package which needs to be
 * referenced from outside the shadows package.
 */
class ShadowMapMaster {

private:
	Window& m_window;
	
	Mat4 m_projectionMatrix = Mat4::Identity();
	Mat4 m_lightViewMatrix = Mat4::Identity();

	ShadowBox m_shadowBox;
	ShadowFrameBuffer m_shadowFBO;

	ShadowShader m_shadowShader = ShadowShader();
	ShadowMapEntityRenderer m_shadowEntityRenderer;

public:
	/*
	 * Creates instances of the important objects needed for rendering the scene
	 * to the shadow map. This includes the {@link ShadowBox} which calculates
	 * the position and size of the "view cuboid", the simple renderer and
	 * shader program that are used to render objects to the shadow map, and the
	 * {@link ShadowFrameBuffer} to which the scene is rendered. The size of the
	 * shadow map is determined here.
	 * 
	 * @param camera
	 *            - the camera being used in the scene.
	 */
	ShadowMapMaster(Window& t_window, Camera& t_camera) 
	: m_window(t_window), m_shadowFBO(t_window), m_shadowBox(), m_shadowEntityRenderer(m_shadowShader) {  }

	GLuint GetShadowMap() { return m_shadowFBO.GetShadowMap(); }
	Mat4 GetLightSpaceTransform() { return m_lightViewMatrix; }
	/*
	* This biased projection-view matrix is used to convert fragments into
	* "shadow map space" when rendering the main render pass. It converts a
	* world space position into a 2D coordinate on the shadow map. This is
	* needed for the second part of shadow mapping.
	*
	* @return The to-shadow-map-space matrix.
	*/
	Mat4 GetToShadowMapSpaceMatrix() { return m_projectionMatrix * m_lightViewMatrix; }

	/*
	 * Carries out the shadow render pass. This renders the entities to the
	 * shadow map. First the shadow box is updated to calculate the size and
	 * position of the "view cuboid". The light direction is assumed to be
	 * "-lightPosition" which will be fairly accurate assuming that the light is
	 * very far from the scene. It then prepares to render, renders the entities
	 * to the shadow map, and finishes rendering.
	 * 
	 * @param entities
	 *            - the lists of entities to be rendered. Each list is
	 *            associated with the {@link TexturedModel} that all of the
	 *            entities in that list use.
	 * @param sun
	 *            - the light acting as the sun in the scene.
	 */
	void Render(const std::map<GLuint, std::list<std::shared_ptr<Entity>>>& t_entities, const std::list<std::shared_ptr<Terrain>>& t_terrains, const Light& t_sun, const Camera& t_camera) {
		//m_shadowBox.SetLightRotationMatrix(m_lightViewMatrix);
		m_shadowBox.Update(t_camera, -t_sun.GetPosition());

		//prepare
		UpdateOrthoProjectionMatrix(m_shadowBox.GetWidth(), m_shadowBox.GetHeight(), m_shadowBox.GetLength());
		m_lightViewMatrix = Mat4::RotationFromDirection(-t_sun.GetPosition()) * Mat4::Translation(-t_camera.GetPosition() - t_camera.GetViewDirection() * SHADOW_DISTANCE / 2.0f/*-m_shadowBox.GetCenter()*/);

		//bind
		m_shadowFBO.BindFrameBuffer();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		m_shadowShader.BindShader();
		m_shadowShader.LoadViewProjectionMatrix(m_projectionMatrix * m_lightViewMatrix);

		//render
		m_shadowEntityRenderer.Render(t_entities, t_terrains);

		//unbind all
		m_shadowShader.UnbindShader();
		m_shadowFBO.UnbindFrameBuffer(m_window);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void CleanUp() {
		m_shadowShader.CleanUp();
		m_shadowFBO.CleanUp();
	}

private:
	/*
	 * Creates the orthographic projection matrix. This projection matrix
	 * basically sets the width, length and height of the "view cuboid", based
	 * on the values that were calculated in the {@link ShadowBox} class.
	 * 
	 * @param width
	 *            - shadow box width.
	 * @param height
	 *            - shadow box height.
	 * @param length
	 *            - shadow box length.
	 */
	void UpdateOrthoProjectionMatrix(float t_width, float t_height, float t_length) {
		m_projectionMatrix = Mat4::Identity();
		
		//the following would be the call to the original function but since only a couple values change it is faster to do it this way
		//Mat4::Orthographic(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -length / 2.0f, length / 2.0f);

		m_projectionMatrix[0] = 2.0f / t_width;
		m_projectionMatrix[5] = 2.0f / t_height;
		//not -2.0f because something, I put it here because it acctually reversed close and far
		//far which should have been drawn white was drawn black and vice versa
		//basically either change this or in the shadowbox maxZ-minZ would become minZ-maxZ
		//this is possiblly a side effect of how openGL handles coordinates
		m_projectionMatrix[10] = -2.0f / t_length;
		m_projectionMatrix[15] = 1;
	}
};
