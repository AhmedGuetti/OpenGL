#include "Cube.h"
#include "Renderer.h"

Cube::Cube(float width, float height, float depth)
    : m_Width(width), m_Height(height), m_Depth(depth), m_Color(1.0f, 1.0f, 1.0f, 1.0f)
{
    Generate();
}

Cube::~Cube()
{
}

void Cube::Generate()
{
    m_Vertices.clear();
    m_Indices.clear();

    float halfWidth = m_Width / 2.0f;
    float halfHeight = m_Height / 2.0f;
    float halfDepth = m_Depth / 2.0f;

    // Vertex positions and normals
    // Format: x, y, z, nx, ny, nz

    // Front face (positive Z)
    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f);

    // Back face (negative Z)
    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f);

    // Left face (negative X)
    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    // Right face (positive X)
    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f); m_Vertices.push_back(0.0f);

    // Bottom face (negative Y)
    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(-halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(-1.0f); m_Vertices.push_back(0.0f);

    // Top face (positive Y)
    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(-halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f);

    m_Vertices.push_back(-halfWidth); m_Vertices.push_back(halfHeight); m_Vertices.push_back(halfDepth);
    m_Vertices.push_back(0.0f); m_Vertices.push_back(1.0f); m_Vertices.push_back(0.0f);

    // Indices for the 6 faces (2 triangles per face)
    unsigned int indices[] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Bottom face
        16, 17, 18, 18, 19, 16,
        // Top face
        20, 21, 22, 22, 23, 20
    };

    // Add indices to the vector
    for (unsigned int i = 0; i < 36; i++)
    {
        m_Indices.push_back(indices[i]);
    }

    // Setup OpenGL buffers
    SetupMesh();
}

void Cube::Update()
{
    // Re-generate the mesh with current parameters
    Generate();
}

void Cube::SetDimensions(float width, float height, float depth)
{
    m_Width = width;
    m_Height = height;
    m_Depth = depth;
    Update();
}

void Cube::SetColor(const glm::vec4& color)
{
    m_Color = color;
}