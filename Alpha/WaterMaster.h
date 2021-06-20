#pragma once
#include "WaterRenderer.h"
#include "WaterFBO.h"

class WaterMaster {
	
private:
	WaterShader m_waterShader = WaterShader();
	WaterRenderer m_waterRenderer;

public:
	WaterMaster(const Window& t_window, const Mat4& t_projectionMatrix) : m_waterRenderer(m_waterShader) {
		m_waterShader.BindShader();
		m_waterShader.LoadProjectionMatrix(t_projectionMatrix);
		m_waterShader.LoadDrawDistance(DRAW_DISTANCE);
		m_waterShader.LoadNearFarPlane(0.1f, 1000.0f); //TODO: make better
		m_waterShader.LoadMaxWaterDepth(5.0f);
		m_waterShader.LoadWaveStrength(0.02f);
		m_waterShader.LoadWaterRefraction(0.5f);
		m_waterShader.LoadReflectivity(0.6f);
		m_waterShader.LoadShineDamper(20.0f);
		m_waterShader.UnbindShader();
	}

	void Render(const std::list<std::shared_ptr<WaterTile>>& t_waterTiles, const WaterFBO& t_waterFBO, const Camera& t_camera, const Light& t_sun) {
		m_waterShader.BindShader();
		m_waterRenderer.Render(t_waterTiles, t_camera, t_waterFBO, t_sun);
		m_waterShader.UnbindShader();
	}

	void CleanUp() {
		m_waterShader.CleanUp();
	}
};