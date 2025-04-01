#include "BezierSurface.h"
#include "Renderer.h"
#include <iostream>

BezierSurface::BezierSurface(unsigned int resolutionU, unsigned int resolutionV)
    : m_ResolutionU(resolutionU), m_ResolutionV(resolutionV),
    m_NumControlPointsU(0), m_NumControlPointsV(0)
{
    // Create a default surface when initialized
    CreateDefaultSurface();
}

BezierSurface::~BezierSurface()
{
}

void BezierSurface::CreateDefaultSurface()
{
    // Create a 4x4 grid of control points
    m_NumControlPointsU = 4;
    m_NumControlPointsV = 4;

    // Clear existing control points
    m_ControlPoints.clear();
    m_ControlPoints.resize(m_NumControlPointsU);

    // Initialize control points
    for (unsigned int i = 0; i < m_NumControlPointsU; i++)
    {
        m_ControlPoints[i].resize(m_NumControlPointsV);
        float u = (float)i / (m_NumControlPointsU - 1);

        for (unsigned int j = 0; j < m_NumControlPointsV; j++)
        {
            float v = (float)j / (m_NumControlPointsV - 1);

            float x = (u - 0.5f) * 2.0f;
            float y = (v - 0.5f) * 2.0f;
            float z = 0.0f;

            // To make it 3D we change z based on the position of the control point
            if ((i == 0 || i == m_NumControlPointsU - 1) && (j == 0 || j == m_NumControlPointsV - 1)) {
                z = 0.5f;
            }
            else if (i == m_NumControlPointsU / 2 && j == m_NumControlPointsV / 2) {
                z = -0.8f;
            }
            else if ((i == m_NumControlPointsU / 2 && (j == 0 || j == m_NumControlPointsV - 1)) ||
                (j == m_NumControlPointsV / 2 && (i == 0 || i == m_NumControlPointsU - 1))) {
                z = 0.8f;
            }

            m_ControlPoints[i][j] = glm::vec3(x, y, z);
        }
    }

    // Generate the mesh
    Generate();
}

void BezierSurface::Generate()
{
    if (m_NumControlPointsU < 2 || m_NumControlPointsV < 2)
        return;

    m_Vertices.clear();
    m_Indices.clear();

    for (unsigned int i = 0; i <= m_ResolutionU; i++)
    {
        float u = (float)i / m_ResolutionU;

        for (unsigned int j = 0; j <= m_ResolutionV; j++)
        {
            float v = (float)j / m_ResolutionV;

            glm::vec3 point = CalculatePoint(u, v);
            glm::vec3 normal = CalculateNormal(u, v);

            m_Vertices.push_back(point.x);
            m_Vertices.push_back(point.y);
            m_Vertices.push_back(point.z);

            m_Vertices.push_back(normal.x);
            m_Vertices.push_back(normal.y);
            m_Vertices.push_back(normal.z);
        }
    }

    // For better visualization, each area in our surface we define the two triangles defining the surface
    for (unsigned int i = 0; i < m_ResolutionU; i++)
    {
        for (unsigned int j = 0; j < m_ResolutionV; j++)
        {
            unsigned int p0 = i * (m_ResolutionV + 1) + j;
            unsigned int p1 = p0 + 1;
            unsigned int p2 = (i + 1) * (m_ResolutionV + 1) + j;
            unsigned int p3 = p2 + 1;

            // Triangle 1
            m_Indices.push_back(p0);
            m_Indices.push_back(p2);
            m_Indices.push_back(p1);

            // Triangle 2
            m_Indices.push_back(p1);
            m_Indices.push_back(p2);
            m_Indices.push_back(p3);
        }
    }

    // Setup OpenGL buffers
    SetupMesh();
}

void BezierSurface::Update()
{
    // Re-generate the mesh with current parameters
    Generate();
}

std::vector<float> BezierSurface::GetFlattenedControlPoints() const
{
    std::vector<float> flattenedPoints;

    for (const auto& row : m_ControlPoints)
    {
        for (const auto& point : row)
        {
            flattenedPoints.push_back(point.x);
            flattenedPoints.push_back(point.y);
            flattenedPoints.push_back(point.z);
        }
    }

    return flattenedPoints;
}

std::vector<float> BezierSurface::GetControlPointGridLines() const
{
    std::vector<float> gridLines;

    // Horizontal grid lines
    for (unsigned int i = 0; i < m_NumControlPointsU; i++)
    {
        for (unsigned int j = 0; j < m_NumControlPointsV - 1; j++)
        {
            gridLines.push_back(m_ControlPoints[i][j].x);
            gridLines.push_back(m_ControlPoints[i][j].y);
            gridLines.push_back(m_ControlPoints[i][j].z);

            gridLines.push_back(m_ControlPoints[i][j + 1].x);
            gridLines.push_back(m_ControlPoints[i][j + 1].y);
            gridLines.push_back(m_ControlPoints[i][j + 1].z);
        }
    }

    // Vertical grid lines
    for (unsigned int j = 0; j < m_NumControlPointsV; j++)
    {
        for (unsigned int i = 0; i < m_NumControlPointsU - 1; i++)
        {
            gridLines.push_back(m_ControlPoints[i][j].x);
            gridLines.push_back(m_ControlPoints[i][j].y);
            gridLines.push_back(m_ControlPoints[i][j].z);

            gridLines.push_back(m_ControlPoints[i + 1][j].x);
            gridLines.push_back(m_ControlPoints[i + 1][j].y);
            gridLines.push_back(m_ControlPoints[i + 1][j].z);
        }
    }

    return gridLines;
}

glm::vec3 BezierSurface::CalculatePoint(float u, float v) const
{
    std::vector<glm::vec3> tempPoints;

    for (unsigned int j = 0; j < m_NumControlPointsV; j++)
    {
        std::vector<glm::vec3> uPoints;
        for (unsigned int i = 0; i < m_NumControlPointsU; i++)
        {
            uPoints.push_back(m_ControlPoints[i][j]);
        }
        tempPoints.push_back(DeCasteljau(uPoints, u));
    }

    // Then use these points to calculate the final point in the v direction
    return DeCasteljau(tempPoints, v);
}

glm::vec3 BezierSurface::DeCasteljau(const std::vector<glm::vec3>& points, float t) const
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

glm::vec3 BezierSurface::CalculateNormal(float u, float v) const
{
    const float delta = 0.01f;

    glm::vec3 point = CalculatePoint(u, v);

    // Calculate partial derivatives
    glm::vec3 du;
    if (u + delta <= 1.0f)
        du = CalculatePoint(u + delta, v) - point;
    else
        du = point - CalculatePoint(u - delta, v);

    glm::vec3 dv;
    if (v + delta <= 1.0f)
        dv = CalculatePoint(u, v + delta) - point;
    else
        dv = point - CalculatePoint(u, v - delta);

    // Cross product to get normal
    glm::vec3 normal = glm::cross(du, dv);
    if (glm::length(normal) > 0.0001f)
        normal = glm::normalize(normal);
    else
        normal = glm::vec3(0.0f, 0.0f, 1.0f); // Default normal if calculation failed

    return normal;
}