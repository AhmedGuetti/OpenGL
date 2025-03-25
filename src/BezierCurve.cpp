#include "BezierCurve.h"

BezierCurve::BezierCurve(unsigned int resolution)
    : m_Resolution(resolution)
{
}

BezierCurve::~BezierCurve()
{
}

void BezierCurve::AddControlPoint(const glm::vec2& point)
{
    m_ControlPoints.push_back(point);
    UpdateCurve();
}

void BezierCurve::ClearControlPoints()
{
    m_ControlPoints.clear();
    m_CurvePoints.clear();
}

void BezierCurve::UpdateCurve()
{
    if (m_ControlPoints.size() < 2)
        return;

    m_CurvePoints.clear();

    
    for (unsigned int i = 0; i <= m_Resolution; i++)
    {
        float t = static_cast<float>(i) / static_cast<float>(m_Resolution);
        glm::vec2 point = CalculatePoint(t);

        // Add point to flattened array
        m_CurvePoints.push_back(point.x);
        m_CurvePoints.push_back(point.y);
    }
}





void BezierCurve::UpdateCurveUniformLength(float segmentLength)
{
    if (m_ControlPoints.size() < 2)
        return;

    m_CurvePoints.clear();

    // Premier point (t = 0)
    float t = 0.0f;
    glm::vec2 currentPoint = CalculatePoint(t);
    m_CurvePoints.push_back(currentPoint.x);
    m_CurvePoints.push_back(currentPoint.y);

    while (t < 1.0f)
    {
        float nextT = FindNextT(t, currentPoint, segmentLength);

        if (nextT <= t)
            break;

        t = nextT;

        currentPoint = CalculatePoint(t);
        m_CurvePoints.push_back(currentPoint.x);
        m_CurvePoints.push_back(currentPoint.y);
    }

    if (t < 1.0f)
    {
        glm::vec2 lastPoint = CalculatePoint(1.0f);
        m_CurvePoints.push_back(lastPoint.x);
        m_CurvePoints.push_back(lastPoint.y);
    }
}

float BezierCurve::FindNextT(float currentT, const glm::vec2& currentPoint, float targetLength)
{
    const float epsilon = 0.001f;
    const float maxStep = 0.1f;
    float t = currentT + epsilon;

    while (t <= 1.0f)
    {
        glm::vec2 testPoint = CalculatePoint(t);
        float distance = glm::length(testPoint - currentPoint);

        if (std::abs(distance - targetLength) < epsilon || distance > targetLength)
            return t;

        t += std::min(maxStep, epsilon * targetLength / distance);
    }

    return 1.0f;
}




const std::vector<float>& BezierCurve::GetCurvePoints() const
{
    return m_CurvePoints;
}

unsigned int BezierCurve::GetPointCount() const
{
    return (m_CurvePoints.size() / 2);
}

const std::vector<glm::vec2>& BezierCurve::GetControlPoints() const
{
    return m_ControlPoints;
}

glm::vec2 BezierCurve::CalculatePoint(float t)
{
    return DeCasteljau(m_ControlPoints, t);
}

glm::vec2 BezierCurve::DeCasteljau(const std::vector<glm::vec2>& points, float t)
{
    if (points.size() == 1)
        return points[0];

    std::vector<glm::vec2> newPoints;
    for (size_t i = 0; i < points.size() - 1; i++)
    {
        glm::vec2 newPoint = (1.0f - t) * points[i] + t * points[i + 1];
        newPoints.push_back(newPoint);
    }

    return DeCasteljau(newPoints, t);
}