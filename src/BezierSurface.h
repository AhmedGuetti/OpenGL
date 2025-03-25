#pragma once

#include <vector>
#include <glm/glm.hpp>

// https://www.scratchapixel.com/lessons/geometry/bezier-curve-rendering-utah-teapot/bezier-surface.html

class BezierSurface {
	private:
		std::vector<std::vector<glm::vec3>> m_ControlPoints; // 2-d grid 
		std::vector<float> m_SurfacePosition;

		unsigned int m_ResolutionU;
		unsigned int m_ResolutionV;
	public:
		BezierSurface(unsigned int resU, unsigned int resV);

		~BezierSurface();

		void AddControlPoint(unsigned int row, unsigned int col, const glm::vec3& point);

		void ClearControlPoints();

		void UpdateCurve();

		// Getters
		const std::vector<float>& GetCurvePoints() const;

		unsigned int GetPointCount() const;
		
		const std::vector<std::vector<glm::vec3>>& GetControlPoints() const;



	private:
		glm::vec3 CalculatePoint(float u, float v);

		glm::vec3 DeCasteljau(const std::vector<glm::vec3>& points, float t);
};