#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "WorldConstants.h"
#include "TextureLoader.h"

/*Class used for generating random terrain height using perlin noise.*/
class HeightGenerator {

private:
	const int m_seed;
	//maximum and minimum height
	const float m_amplitude; //usually around 70.0f
	//number of curves used, each with an increased frequency, but a decreased contribution to the altitude
	const int m_octaves; //usually around 3
	//the starting curve's smoothness
	const float m_baseSmoothness; //usually a multiple of 2 around 8, 16, 32
	//how much each following curve will contribute to the altitude
	const float m_roughness; //usually around 0.3f

public:
	HeightGenerator(const int t_seed, float t_amplitude, const int t_octaves, const float t_baseSmoothness, const float t_roughness) 
		: m_seed(t_seed), m_amplitude(t_amplitude), m_octaves(t_octaves), m_baseSmoothness(t_baseSmoothness), m_roughness(t_roughness) { }

	float GenerateHeight(int x, int z) const {
		if (m_seed == FLAT_SEED) return 0;

		int xPos = x + INT_MAX / 500;
		int zPos = z + INT_MAX / 500;

		float total = 0;

		for (int x = 0; x < m_octaves; x++) {
			float freq = (float)(pow(2, x) / m_baseSmoothness);
			float amp = (float)pow(m_roughness, x) * m_amplitude;
			total += GetInterpolatedNoise(xPos * freq, zPos * freq) * amp;
		}

		return total;
	}
	unsigned char* GetHeightFromImage(const std::string& t_filename) const {
		unsigned char* pixels = nullptr;
		TextureLoader::GetTexturePixels(t_filename, pixels);
		return pixels;
	}

private:
	float GetInterpolatedNoise(float x, float z) const {
		int intX = (int)x;
		int intZ = (int)z;
		float fracX = x - intX;
		float fracZ = z - intZ;

		float v1 = GetSmoothNoise(intX, intZ);
		float v2 = GetSmoothNoise(intX + 1, intZ);
		float v3 = GetSmoothNoise(intX, intZ + 1);
		float v4 = GetSmoothNoise(intX + 1, intZ + 1);
		float i1 = Interpolate(v1, v2, fracX);
		float i2 = Interpolate(v3, v4, fracX);
		return Interpolate(i1, i2, fracZ);
	}
	float Interpolate(float a, float b, float blend) const {
		double theta = blend * M_PI;
		float f = (float)(1.0f - cos(theta)) * 0.5f;
		return a * (1.0f - f) + b * f;
	}
	float GetSmoothNoise(int x, int z) const {
		float corners = (GetNoise(x + 1, z + 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z - 1) + GetNoise(x - 1, z + 1)) / 16.0f;
		float sides = (GetNoise(x + 1, z) + GetNoise(x - 1, z) + GetNoise(x, z + 1) + GetNoise(x, z - 1)) / 8.0f;
		float center = GetNoise(x, z) / 4.0f;
		return corners + sides + center;
	}
	float GetNoise(int x, int z) const  {
		int n = x * 347 + z * 57 + m_seed;
		n = (n << 13) ^ n;
		return (1.0f - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}
};