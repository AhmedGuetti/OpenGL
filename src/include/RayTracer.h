#pragma once

#include "Ray.h"
#include "Camera.h"
#include "Shape.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Sphere.h"
#include <vector>
#include <memory>

class RayTracer
{
private:
    Camera& m_Camera;
    std::vector<Shape*> m_Shapes;

    // Current ray to visualize
    bool m_ShowRay;
    Ray m_CurrentRay;
    float m_RayLength;
    glm::vec3 m_HitPosition;
    glm::vec3 m_HitNormal;

    // OpenGL objects for ray visualization
    std::unique_ptr<VertexArray> m_RayVAO;
    std::unique_ptr<VertexBuffer> m_RayVBO;

    // OpenGL objects for hit point visualization
    std::unique_ptr<VertexArray> m_HitPointVAO;
    std::unique_ptr<VertexBuffer> m_HitPointVBO;

    // OpenGL objects for normal visualization
    std::unique_ptr<VertexArray> m_NormalVAO;
    std::unique_ptr<VertexBuffer> m_NormalVBO;

    // Helper method for sphere intersection
    bool IntersectSphere(const Ray& ray, const Sphere& sphere, float& t);

public:
    RayTracer(Camera& camera);
    ~RayTracer();

    // Add a shape to the scene
    void AddShape(Shape* shape);

    // Generate ray through screen coordinates
    Ray GenerateRay(float screenX, float screenY, int screenWidth, int screenHeight);

    // Set current ray for visualization
    void SetCurrentRay(const Ray& ray);

    // Test ray intersection with all shapes
    bool Intersect(const Ray& ray, float& t, int& shapeIndex);

    // Render the current ray
    void RenderRay(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
};