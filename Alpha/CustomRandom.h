#pragma once

class CustomRandom {
	int m_seed;
	int m_current;

public:
	CustomRandom(int seed) {
		m_seed = seed;
		m_current = seed;
	}

	float GetNoise() {
		m_current = m_current * 347;
		return generateRandom(m_current);
	}

	float GetNoise(int x) {
		int n = x * 347 + m_seed;
		return generateRandom(n);
	}

	float GetNoise(int x, int z) {
		int n = x * 347 + z * 57 + m_seed;
		return generateRandom(n);
	}

private:
	float generateRandom(int n) {
		n = (n << 13) ^ n;
		return (1.0f - ((n * ((n * n * 15731) + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	}
};