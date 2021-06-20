#include "Math.h"
#include "WorldConstants.h"

/**
 * Represents the 3D cuboidal area of the world in which objects will cast
 * shadows (basically represents the orthographic projection area for the shadow
 * render pass). It is updated each frame to optimise the area, making it as
 * small as possible (to allow for optimal shadow map resolution) while not
 * being too small to avoid objects not having shadows when they should.
 * Everything inside the cuboidal area represented by this object will be
 * rendered to the shadow map in the shadow render pass. Everything outside the
 * area won't be.
 */
class ShadowBox {

private:
	float m_offset = 10.0f; //the padding on the shadowbox so when the sun is low shadows from objects a bit outside your view are still cast
	const float m_frustumLength = SHADOW_DISTANCE; //not the actual view frustum just the length after which shadows aren't rendered
	//this can be just the rotation because the dimensions of the shadowbox don't change if you translate it
	//so you can just consider the rotation due to the light direction and the camera rotation and also 
	//you take into account camera translation so you can get the center, you have to calculate the center and
	//not just take camera translation + camera view * SHADOW_DISTANCE because of the offsets and because it doesn't work
	//the centers of the camera frustum and the shadow box don't line up
	Mat4 m_inverseLightRotationMatrix = Mat4::Identity(); 
	

	//the positions of the frustum points which reach max x, y and z
	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	//these exist so you don't have to calculate them on every update
	float m_frusutmFarHeight;
	float m_frustumFarWidth;
	float m_frustumNearHeight;
	float m_frustumNearWidth;

public:
	/**
	 * Creates a new shadow box and calculates some initial values relating to
	 * the camera's view frustum, namely the width and height of the near plane
	 * and (possibly adjusted) far plane.
	 * 
	 * @param lightViewMatrix
	 *            - basically the "view matrix" of the light. Can be used to
	 *            transform a point from world space into "light" space (i.e.
	 *            changes a point's coordinates from being in relation to the
	 *            world's axis to being in terms of the light's local axis).
	 * @param camera
	 *            - the in-game camera.
	 */
	ShadowBox() {
		//just draw a triangle and try working out the far side's length from the near side's angle
		m_frustumFarWidth = (float)(2 * m_frustumLength * tan((PERSPECTIVE_FOV * M_PI / 180.0f) / 2.0f));
		m_frustumNearWidth = (float)(2 * PERSPECTIVE_NEAR * tan((PERSPECTIVE_FOV * M_PI / 180.0f) / 2.0f));

		//you want to paste the view frustum to the screen and the ratio of the width and height has to match the screen's ratio
		float aspectRatio = Display::GetAspectRatio();
		m_frusutmFarHeight = m_frustumFarWidth / aspectRatio;
		m_frustumNearHeight = m_frustumNearWidth / aspectRatio;
	}

	/*
	 * Updates the bounds of the shadow box based on the light direction and the
	 * camera's view frustum, to make sure that the box covers the smallest area
	 * possible while still ensuring that everything inside the camera's view
	 * (within a certain range of ViewFrustumLength) will cast shadows.
	 */
	void Update(const Camera& t_camera, const Vec3& t_lightDirection) {
		m_inverseLightRotationMatrix = Mat4::Invert(Mat4::RotationFromDirection(t_lightDirection));
		std::vector<Vec3> points = CalculateFrustumVertices(t_camera); //in light space

		/*Find the min and max x, y and z value of the frustum in relation to light space.
		Use this info to construct the shadow box around the frustum.*/
		bool first = true;
		for (Vec3 point : points) {
			if (first) {
				minX = point.x;
				maxX = point.x;
				minY = point.y;
				maxY = point.y;
				minZ = point.z;
				maxZ = point.z;
				first = false;
				continue;
			}
			if (point.x > maxX) maxX = point.x;
			else if (point.x < minX) minX = point.x;

			if (point.y > maxY) maxY = point.y;
			else if (point.y < minY) minY = point.y;

			if (point.z > maxZ) maxZ = point.z;
			else if (point.z < minZ) minZ = point.z;
		}

		//offset for shadows of objects from behind you
		//since the center depends on these paramaters the offset that is applied here
		//affects the actual box as you would expect
		//maxX += m_offset;
		//maxY += m_offset;
		//maxZ += m_offset;
		//minX -= m_offset;
		//minY -= m_offset;
		minZ -= m_offset;
	}

	/*
	 * Calculates the center of the "view cuboid" in light space first, and then
	 * converts this to world space using the inverse light's view matrix.
	 * 
	 * @return The center of the "view cuboid" in world space.
	 */
	Vec3 GetCenter() const {
		float x = (minX + maxX) / 2.0f;
		float y = (minY + maxY) / 2.0f;
		float z = (minZ + maxZ) / 2.0f;
		
		return Mat4::Invert(m_inverseLightRotationMatrix) * Vec3(x, y, z);
	}
	float GetWidth() const { return maxX - minX; }
	float GetHeight() const { return maxY - minY; }
	float GetLength() const { return maxZ - minZ; }

private:
	/**
	 * Calculates the position of the vertex at each corner of the view frustum
	 * in light space (8 vertices in total, so this returns 8 positions).
	 * 
	 */
	std::vector<Vec3> CalculateFrustumVertices(const Camera& t_camera) {
		std::vector<Vec3> frustumPoints;
		//Mat4 camViewMat = Mat4::Translation(t_camera.GetPosition()) * Mat4::Invert(Mat4::RotationFromDirection(-t_camera.GetViewDirection()));
		Mat4 camViewMat = t_camera.GetViewMatrix();

		//calculate points in world space
		frustumPoints.push_back(camViewMat * Vec3(m_frustumFarWidth / 2.0f, m_frusutmFarHeight / 2.0f, m_frustumLength + PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(-m_frustumFarWidth / 2.0f, m_frusutmFarHeight / 2.0f, m_frustumLength + PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(m_frustumFarWidth / 2.0f, -m_frusutmFarHeight / 2.0f, m_frustumLength + PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(-m_frustumFarWidth / 2.0f, -m_frusutmFarHeight / 2.0f, m_frustumLength + PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(m_frustumNearWidth / 2.0f, m_frustumNearHeight / 2.0f, PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(-m_frustumNearWidth / 2.0f, m_frustumNearHeight / 2.0f, PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(m_frustumNearWidth / 2.0f, -m_frustumNearHeight / 2.0f, PERSPECTIVE_NEAR));
		frustumPoints.push_back(camViewMat * Vec3(-m_frustumNearWidth / 2.0f, -m_frustumNearHeight / 2.0f, PERSPECTIVE_NEAR));

		for (unsigned int x = 0; x < frustumPoints.size(); x++) {
			//the points are rotated by the inverse light rotation matrix because maths.
			//I tested it on paper a couble of times and it seems to work, if you start from an arbitrary
			//view frustum and then you rotate it by the negative angle of the light direction vector 
			//(i am talking about a 2D case where you rotate around the origin, but i guess it also applies to 3D)
			//and you consider the light direction to be a vector representing the positive x axis of your new coordinate system
			frustumPoints[x] = m_inverseLightRotationMatrix * frustumPoints[x];
		}

		return frustumPoints;
	}
};
