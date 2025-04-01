#include "RayTracer.h"
#include "Renderer.h"
#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

RayTracer::RayTracer(Camera& camera)
    : m_Camera(camera), m_ShowRay(false), m_CurrentRay(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
    m_RayLength(100.0f), m_HitPosition(glm::vec3(0.0f)), m_HitNormal(glm::vec3(0.0f))
{
}

RayTracer::~RayTracer()
{
}

void RayTracer::AddShape(Shape* shape)
{
    if (shape)
        m_Shapes.push_back(shape);
}

Ray RayTracer::GenerateRay(float screenX, float screenY, int screenWidth, int screenHeight)
{
    // Convert screen coordinates to normalized device coordinates [-1, 1]
    float ndcX = (2.0f * screenX) / screenWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * screenY) / screenHeight; // Flip Y for OpenGL

    // Create clip space position (NDC with homogeneous coordinate)
    glm::vec4 clipPos = glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

    // Convert to view space
    glm::mat4 projInverse = glm::inverse(glm::perspective(glm::radians(m_Camera.GetZoom()),
        (float)screenWidth / (float)screenHeight, 0.1f, 100.0f));
    glm::vec4 viewPos = projInverse * clipPos;
    viewPos = glm::vec4(viewPos.x, viewPos.y, -1.0f, 0.0f); // Point in view direction with w=0

    // Convert to world space
    glm::mat4 viewInverse = glm::inverse(m_Camera.GetViewMatrix());
    glm::vec4 worldPos = viewInverse * viewPos;

    // Create and normalize the ray direction
    glm::vec3 direction = glm::normalize(glm::vec3(worldPos));

    // Return ray starting at camera position, going in the calculated direction
    return Ray(m_Camera.GetPosition(), direction);
}

void RayTracer::SetCurrentRay(const Ray& ray)
{
    m_CurrentRay = ray;
    m_ShowRay = true;

    // Test for intersection
    float t;
    int shapeIndex;
    if (Intersect(ray, t, shapeIndex)) {
        std::cout << "Ray hit shape at distance: " << t << std::endl;
        m_RayLength = t;
        m_HitPosition = ray.GetPointAt(t);

        // For a sphere, the normal is the normalized vector from sphere center to hit point
        if (shapeIndex >= 0 && shapeIndex < m_Shapes.size()) {
            if (Sphere* sphere = dynamic_cast<Sphere*>(m_Shapes[shapeIndex])) {
                m_HitNormal = glm::normalize(m_HitPosition - sphere->GetPosition());

                // Print intersection info
                std::cout << "Hit position: ("
                    << m_HitPosition.x << ", "
                    << m_HitPosition.y << ", "
                    << m_HitPosition.z << ")" << std::endl;

                std::cout << "Surface normal: ("
                    << m_HitNormal.x << ", "
                    << m_HitNormal.y << ", "
                    << m_HitNormal.z << ")" << std::endl;
            }
        }
    }
    else {
        std::cout << "Ray did not hit any object" << std::endl;
        m_RayLength = 100.0f; // Default ray length
        m_HitPosition = ray.GetPointAt(m_RayLength);
        m_HitNormal = glm::vec3(0.0f);
    }
}

bool RayTracer::Intersect(const Ray& ray, float& t, int& shapeIndex)
{
    t = FLT_MAX;
    shapeIndex = -1;
    bool hit = false;

    // Test intersection with each shape
    for (size_t i = 0; i < m_Shapes.size(); i++) {
        float tShape;

        if (Sphere* sphere = dynamic_cast<Sphere*>(m_Shapes[i])) {
            if (IntersectSphere(ray, *sphere, tShape)) {
                if (tShape < t) {
                    t = tShape;
                    shapeIndex = i;
                    hit = true;
                }
            }
        }
    }

    return hit;
}

bool RayTracer::IntersectSphere(const Ray& ray, const Sphere& sphere, float& t)
{
    // Get sphere center and radius
    glm::vec3 center = sphere.GetPosition();
    float radius = sphere.GetRadius();

    // Vector from ray origin to sphere center
    glm::vec3 oc = ray.GetOrigin() - center;

    // Quadratic formula coefficients
    float a = glm::dot(ray.GetDirection(), ray.GetDirection());
    float b = 2.0f * glm::dot(oc, ray.GetDirection());
    float c = glm::dot(oc, oc) - radius * radius;

    // Calculate discriminant
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false; // No intersection

    // Find the nearest intersection point
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    // Choose closest intersection point in front of the camera
    if (t1 > 0.001f) {
        t = t1;
        return true;
    }

    if (t2 > 0.001f) {
        t = t2;
        return true;
    }

    return false; // Ray origin is inside sphere or intersections are behind ray
}

void RayTracer::RenderRay(Shader& shader, const glm::mat4& view, const glm::mat4& projection)
{
    if (!m_ShowRay)
        return;

    // Calculate ray end point
    glm::vec3 rayEnd = m_CurrentRay.GetPointAt(m_RayLength);

    // Create ray line vertices
    std::vector<float> vertices = {
        m_CurrentRay.GetOrigin().x, m_CurrentRay.GetOrigin().y, m_CurrentRay.GetOrigin().z,
        rayEnd.x, rayEnd.y, rayEnd.z
    };

    // Create or update buffers for ray
    if (!m_RayVAO) {
        m_RayVAO = std::make_unique<VertexArray>();
        m_RayVBO = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3); // Position only
        m_RayVAO->AddBuffer(*m_RayVBO, layout);
    }
    else {
        m_RayVBO->Bind();
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW));
    }

    // Draw ray
    shader.Bind();
    shader.SetUniformMat4f("u_Model", glm::mat4(1.0f));
    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Projection", projection);
    shader.SetUniform4f("u_Color", 1.0f, 1.0f, 0.0f, 1.0f); // Yellow ray

    m_RayVAO->Bind();
    GLCall(glLineWidth(3.0f));
    GLCall(glDrawArrays(GL_LINES, 0, 2));

    // If there's a hit, visualize the hit point and normal
    if (m_RayLength < 100.0f) {
        // Create a small point to show hit position
        std::vector<float> pointVertex = {
            m_HitPosition.x, m_HitPosition.y, m_HitPosition.z
        };

        if (!m_HitPointVAO) {
            m_HitPointVAO = std::make_unique<VertexArray>();
            m_HitPointVBO = std::make_unique<VertexBuffer>(pointVertex.data(), pointVertex.size() * sizeof(float));

            VertexBufferLayout pointLayout;
            pointLayout.Push<float>(3);
            m_HitPointVAO->AddBuffer(*m_HitPointVBO, pointLayout);
        }
        else {
            m_HitPointVBO->Bind();
            GLCall(glBufferData(GL_ARRAY_BUFFER, pointVertex.size() * sizeof(float), pointVertex.data(), GL_STATIC_DRAW));
        }

        // Draw hit point
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f); // Red hit point
        m_HitPointVAO->Bind();
        GLCall(glPointSize(10.0f));
        GLCall(glDrawArrays(GL_POINTS, 0, 1));

        // Draw normal vector at hit point
        if (glm::length(m_HitNormal) > 0.01f) {
            glm::vec3 normalEnd = m_HitPosition + m_HitNormal * 0.5f; // Scale normal for visibility

            std::vector<float> normalVertex = {
                m_HitPosition.x, m_HitPosition.y, m_HitPosition.z,
                normalEnd.x, normalEnd.y, normalEnd.z
            };

            if (!m_NormalVAO) {
                m_NormalVAO = std::make_unique<VertexArray>();
                m_NormalVBO = std::make_unique<VertexBuffer>(normalVertex.data(), normalVertex.size() * sizeof(float));

                VertexBufferLayout normalLayout;
                normalLayout.Push<float>(3);
                m_NormalVAO->AddBuffer(*m_NormalVBO, normalLayout);
            }
            else {
                m_NormalVBO->Bind();
                GLCall(glBufferData(GL_ARRAY_BUFFER, normalVertex.size() * sizeof(float), normalVertex.data(), GL_STATIC_DRAW));
            }

            // Draw normal
            shader.SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f); // Green normal
            m_NormalVAO->Bind();
            GLCall(glLineWidth(2.0f));
            GLCall(glDrawArrays(GL_LINES, 0, 2));
        }
    }

    // Reset OpenGL state
    GLCall(glLineWidth(1.0f));
    GLCall(glPointSize(1.0f));
}