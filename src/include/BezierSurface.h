#pragma once

#include "Shape.h"
#include <vector>

class BezierSurface : public Shape
{
private:
    std::vector<std::vector<glm::vec3>> m_ControlPoints;
    unsigned int m_ResolutionU;
    unsigned int m_ResolutionV;
    unsigned int m_NumControlPointsU;
    unsigned int m_NumControlPointsV;

public:
    BezierSurface(unsigned int resolutionU = 20, unsigned int resolutionV = 20);
    ~BezierSurface() override;

    // Inherited from Shape
    void Generate() override;
    void Update() override;

    // BezierSurface-specific methods
    void CreateDefaultSurface();
    std::vector<float> GetFlattenedControlPoints() const;
    std::vector<float> GetControlPointGridLines() const;

    // Getters
    const std::vector<std::vector<glm::vec3>>& GetControlPoints() const { return m_ControlPoints; }

private:
    // De Casteljau algorithm for a 1D Bezier curve
    glm::vec3 DeCasteljau(const std::vector<glm::vec3>& points, float t) const;

    // Calculate point on the surface at parameters u, v
    glm::vec3 CalculatePoint(float u, float v) const;

    // Calculate surface normal at a point
    glm::vec3 CalculateNormal(float u, float v) const;
};