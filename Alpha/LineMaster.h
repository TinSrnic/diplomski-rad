#pragma once
#include "LineRenderer.h"

class LineMaster {

private:
	LineShader m_lineShader = LineShader();
	LineRenderer m_lineRenderer;

public:
	LineMaster(const Mat4& t_projectionMatrix) : m_lineRenderer(m_lineShader) {
		m_lineShader.BindShader();
		m_lineShader.LoadProjectionMatrix(t_projectionMatrix);
		glLineWidth(LINE_WIDTH);
		m_lineShader.UnbindShader();
	}

	void Render(const std::vector<std::shared_ptr<Line>>& t_lines, const Mat4& t_viewMatrix) {
 		m_lineShader.BindShader();
		m_lineShader.LoadViewMatrix(t_viewMatrix);
		m_lineRenderer.Render(t_lines);
		m_lineShader.UnbindShader();
	}

};