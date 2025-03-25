#include "BezierSurface.h"



BezierSurface::BezierSurface(unsigned int resU, unsigned int resV)
	:m_ResolutionU(resU), m_ResolutionV(resV){
	
}


BezierSurface::~BezierSurface() {

}

void BezierSurface::AddControlPoint(unsigned int row, unsigned int col,const glm::vec3& point) {
    // just give it the more spave i think
    if (row >= m_ControlPoints.size()) {
        m_ControlPoints.resize(row + 1);
    }
    if (col >= m_ControlPoints[row].size()) {
        m_ControlPoints[row].resize(col + 1);
    }

    m_ControlPoints[row][col] = point;
}



void BezierSurface::ClearControlPoints() {
	m_ControlPoints.clear();
	m_SurfacePosition.clear();
}

void BezierSurface::UpdateCurve() {

}

const std::vector<float>& BezierSurface::GetCurvePoints() const {

}

unsigned int  BezierSurface::GetPointCount() const {
    return (m_SurfacePosition.size() / 2);
}

const std::vector<std::vector<glm::vec3>>& BezierSurface::GetControlPoints() const {

}

glm::vec3 BezierSurface::CalculatePoint(float u, float v) {
    if (m_ControlPoints.empty()) {
        return glm::vec3(0.0f);
    }

    std::vector<glm::vec3> vCurves;
    for (const auto& row : m_ControlPoints) {
        std::vector<glm::vec3> curvePoints(row.begin(), row.end());
        vCurves.push_back(DeCasteljau(curvePoints, u));
    }
    return DeCasteljau(vCurves, v);
}

glm::vec3 BezierSurface::DeCasteljau(const std::vector<glm::vec3>& points, float t)
{
    if (points.size() == 1)
        return points[0];

    std::vector<glm::vec3> newPoints;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        glm::vec3 newPoint = (1.0f - t) * points[i] + t * points[i + 1];
        newPoints.push_back(newPoint);
    }

    return DeCasteljau(newPoints, t);
}