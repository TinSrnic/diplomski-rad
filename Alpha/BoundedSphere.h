#pragma once
#include "BoundedBody.h"

class BoundedSphere : public BoundedBody {

private:

public:
	BoundedSphere(const Vec3& t_position, const Vec3& t_scale) : BoundedBody(t_position, t_scale) {
		//medium.com/game-dev-daily/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
		//tutorial.math.lamar.edu/Classes/CalcIII/SphericalCoords.aspx
		//gamedev.stackexchange.com/questions/150191/opengl-calculate-uv-sphere-vertices
		
		// One vertex at every latitude-longitude intersection,
		// plus one for the north pole and one for the south.
		// One meridian serves as a UV seam, so we double the vertices there.
		int numLatitudeLines = 6;
		int numLongitudeLines = 6;
		int numVertices = numLatitudeLines * (numLongitudeLines + 1) + 2;
		float radius = 1.0f;

		// North pole.
		m_vertices.push_back(Vec3(0.0f, radius, 0.0f));

		// South pole.
		m_vertices.push_back(Vec3(0.0f, -radius, 0.0f));

		// +1.0f because there's a gap between the poles and the first parallel.
		float latitudeSpacing = 1.0f / (numLatitudeLines + 1.0f);
		float longitudeSpacing = 1.0f / (numLongitudeLines);

		// start writing new vertices at position 1
		for (int latitude = 0; latitude < numLatitudeLines; latitude++) {
			for (int longitude = 0; longitude <= numLongitudeLines; longitude++) {
				// Convert to spherical coordinates:
				// theta is a longitude angle (around the equator) in radians.
				// phi is a latitude angle (north or south of the equator).
				float theta = longitude * longitudeSpacing * 2.0f * M_PI;
				float phi = (1.0f - (latitude + 1) * latitudeSpacing - 0.5f) * M_PI;

				// This determines the radius of the ring of this line of latitude.
				// It's widest at the equator, and narrows as phi increases/decreases.
				float c = cos(phi);

				// Usual formula for a vector in spherical coordinates.
				// You can exchange x & z to wind the opposite way around the sphere.
				m_vertices.push_back(Vec3(c * cos(theta), sin(phi), c * sin(theta)));
			}
		}
	}
};
