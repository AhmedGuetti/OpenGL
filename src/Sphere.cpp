#include "Sphere.h"
#include "Renderer.h"
#include <glm/gtc/constants.hpp>

Sphere::Sphere(float radius, unsigned int sectors, unsigned int stacks)
    : m_Radius(radius), m_Sectors(sectors), m_Stacks(stacks), m_FlatShading(false)
{
    Generate();
}

Sphere::~Sphere()
{
}

void Sphere::Generate()
{
    if (m_FlatShading)
    {
        GenerateFlatShadedSphere();
        return;
    }

    m_Vertices.clear();
    m_Indices.clear();

    float sectorStep = 2 * glm::pi<float>() / m_Sectors;
    float stackStep = glm::pi<float>() / m_Stacks;

    // Generate vertices
    for (unsigned int i = 0; i <= m_Stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;  // starting from pi/2 to -pi/2
        float xy = m_Radius * cosf(stackAngle);                   // r * cos(u)
        float z = m_Radius * sinf(stackAngle);                    // r * sin(u)

        // Add (sectors+1) vertices per stack
        // The first and last vertices have same position and normal, but different tex coords
        for (unsigned int j = 0; j <= m_Sectors; ++j)
        {
            float sectorAngle = j * sectorStep;  // starting from 0 to 2pi

            // Vertex position
            float x = xy * cosf(sectorAngle);    // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle);    // r * cos(u) * sin(v)

            // Normalized vertex normal (for smooth shading)
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));

            // Add position
            m_Vertices.push_back(x);
            m_Vertices.push_back(y);
            m_Vertices.push_back(z);

            // Add normal
            m_Vertices.push_back(normal.x);
            m_Vertices.push_back(normal.y);
            m_Vertices.push_back(normal.z);
        }
    }

    // Generate indices
    for (unsigned int i = 0; i < m_Stacks; ++i)
    {
        unsigned int k1 = i * (m_Sectors + 1);        // beginning of current stack
        unsigned int k2 = k1 + m_Sectors + 1;         // beginning of next stack

        for (unsigned int j = 0; j < m_Sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding the first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                m_Indices.push_back(k1);
                m_Indices.push_back(k2);
                m_Indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (m_Stacks - 1))
            {
                m_Indices.push_back(k1 + 1);
                m_Indices.push_back(k2);
                m_Indices.push_back(k2 + 1);
            }
        }
    }

    // Setup OpenGL buffers
    SetupMesh();
}

void Sphere::GenerateFlatShadedSphere()
{
    m_Vertices.clear();
    m_Indices.clear();

    float sectorStep = 2 * glm::pi<float>() / m_Sectors;
    float stackStep = glm::pi<float>() / m_Stacks;

    // Generate vertex positions first (for calculating face normals)
    std::vector<glm::vec3> positions;

    for (unsigned int i = 0; i <= m_Stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;  // starting from pi/2 to -pi/2
        float xy = m_Radius * cosf(stackAngle);                   // r * cos(u)
        float z = m_Radius * sinf(stackAngle);                    // r * sin(u)

        for (unsigned int j = 0; j <= m_Sectors; ++j)
        {
            float sectorAngle = j * sectorStep;  // starting from 0 to 2pi

            // Vertex position
            float x = xy * cosf(sectorAngle);    // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle);    // r * cos(u) * sin(v)

            positions.push_back(glm::vec3(x, y, z));
        }
    }

    // Now generate triangles with face normals
    unsigned int index = 0;

    // Indices for accessing positions
    for (unsigned int i = 0; i < m_Stacks; ++i)
    {
        unsigned int k1 = i * (m_Sectors + 1);        // beginning of current stack
        unsigned int k2 = k1 + m_Sectors + 1;         // beginning of next stack

        for (unsigned int j = 0; j < m_Sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding the first and last stacks
            if (i != 0)
            {
                // Triangle: k1 => k2 => k1+1
                glm::vec3 v1 = positions[k1];
                glm::vec3 v2 = positions[k2];
                glm::vec3 v3 = positions[k1 + 1];

                // Calculate face normal
                glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

                // Add vertices with the face normal
                m_Vertices.push_back(v1.x); m_Vertices.push_back(v1.y); m_Vertices.push_back(v1.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                m_Vertices.push_back(v2.x); m_Vertices.push_back(v2.y); m_Vertices.push_back(v2.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                m_Vertices.push_back(v3.x); m_Vertices.push_back(v3.y); m_Vertices.push_back(v3.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                // Add indices
                m_Indices.push_back(index++);
                m_Indices.push_back(index++);
                m_Indices.push_back(index++);
            }

            if (i != (m_Stacks - 1))
            {
                // Triangle: k1+1 => k2 => k2+1
                glm::vec3 v1 = positions[k1 + 1];
                glm::vec3 v2 = positions[k2];
                glm::vec3 v3 = positions[k2 + 1];

                // Calculate face normal
                glm::vec3 normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

                // Add vertices with the face normal
                m_Vertices.push_back(v1.x); m_Vertices.push_back(v1.y); m_Vertices.push_back(v1.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                m_Vertices.push_back(v2.x); m_Vertices.push_back(v2.y); m_Vertices.push_back(v2.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                m_Vertices.push_back(v3.x); m_Vertices.push_back(v3.y); m_Vertices.push_back(v3.z);
                m_Vertices.push_back(normal.x); m_Vertices.push_back(normal.y); m_Vertices.push_back(normal.z);

                // Add indices
                m_Indices.push_back(index++);
                m_Indices.push_back(index++);
                m_Indices.push_back(index++);
            }
        }
    }

    // Setup OpenGL buffers
    SetupMesh();
}

void Sphere::Update()
{
    // Re-generate the mesh with current parameters
    Generate();
}

void Sphere::SetRadius(float radius)
{
    m_Radius = radius;
    Update();
}

void Sphere::SetResolution(unsigned int sectors, unsigned int stacks)
{
    m_Sectors = sectors;
    m_Stacks = stacks;
    Update();
}

void Sphere::ToggleFlatShading()
{
    m_FlatShading = !m_FlatShading;
    Update();
}