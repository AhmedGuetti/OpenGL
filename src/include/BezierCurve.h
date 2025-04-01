#pragma once

#include <vector>
#include <glm/glm.hpp>

class BezierCurve
{
private:
    std::vector<glm::vec2> m_ControlPoints;
    std::vector<float> m_CurvePoints; // Flattened array of x,y coordinates
    unsigned int m_Resolution;

public:
    BezierCurve(unsigned int resolution = 100);
    ~BezierCurve();

    void AddControlPoint(const glm::vec2& point);

    void ClearControlPoints();

    void UpdateCurve();


    const std::vector<float>& GetCurvePoints() const;

    unsigned int GetPointCount() const;

    const std::vector<glm::vec2>& GetControlPoints() const;
    
    void UpdateCurveUniformLength(float segmentLength);

    float FindNextT(float currentT, const glm::vec2& currentPoint, float targetLength);

private:
    glm::vec2 CalculatePoint(float t);

    glm::vec2 DeCasteljau(const std::vector<glm::vec2>& points, float t);
};