#include "Shape.h"
#include "Renderer.h"

Shape::Shape()
    : m_Position(0.0f, 0.0f, 0.0f),
    m_Rotation(0.0f, 0.0f, 0.0f),
    m_Scale(1.0f, 1.0f, 1.0f),
    m_WireframeMode(false)
{
}

Shape::~Shape()
{
}

void Shape::SetupMesh()
{
    if (m_Vertices.empty() || m_Indices.empty())
        return;

    // Create vertex array and buffer
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(m_Vertices.data(), m_Vertices.size() * sizeof(float));

    // Setup the vertex buffer layout
    VertexBufferLayout layout;
    layout.Push<float>(3); // Position (3 components: x, y, z)
    layout.Push<float>(3); // Normal (3 components: nx, ny, nz)
    m_VAO->AddBuffer(*m_VBO, layout);

    // Create index buffer
    m_IBO = std::make_unique<IndexBuffer>(m_Indices.data(), m_Indices.size());
}

void Shape::SetPosition(const glm::vec3& position)
{
    m_Position = position;
}

void Shape::SetRotation(const glm::vec3& rotation)
{
    m_Rotation = rotation;
}

void Shape::SetScale(const glm::vec3& scale)
{
    m_Scale = scale;
}

glm::mat4 Shape::GetModelMatrix() const
{
    glm::mat4 model = glm::mat4(1.0f);

    // Apply transformations in this order: scale, rotate, translate
    model = glm::translate(model, m_Position);

    // Apply rotations
    model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, m_Scale);

    return model;
}

void Shape::ToggleWireframe()
{
    m_WireframeMode = !m_WireframeMode;
}

bool Shape::IsWireframe() const
{
    return m_WireframeMode;
}

const std::vector<float>& Shape::GetVertices() const
{
    return m_Vertices;
}

const std::vector<unsigned int>& Shape::GetIndices() const
{
    return m_Indices;
}

unsigned int Shape::GetVertexCount() const
{
    return m_Vertices.size() / 6; // 6 floats per vertex (position + normal)
}

unsigned int Shape::GetIndexCount() const
{
    return m_Indices.size();
}

void Shape::Bind() const
{
    if (m_VAO && m_IBO)
    {
        m_VAO->Bind();
        m_IBO->Bind();
    }
}

void Shape::Unbind() const
{
    if (m_VAO && m_IBO)
    {
        m_VAO->Unbind();
        m_IBO->Unbind();
    }
}

void Shape::Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const
{
    if (!m_VAO || !m_IBO)
        return;

    shader.Bind();

    // Set transformation matrices
    glm::mat4 model = GetModelMatrix();
    shader.SetUniformMat4f("u_Model", model);
    shader.SetUniformMat4f("u_View", view);
    shader.SetUniformMat4f("u_Projection", projection);

    // Bind vertex array and index buffer
    Bind();

    // Set wireframe mode if enabled
    if (m_WireframeMode)
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }

    // Draw the shape
    GLCall(glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, nullptr));

    // Reset polygon mode
    if (m_WireframeMode)
    {
        GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    // Unbind
    Unbind();
    shader.Unbind();
}