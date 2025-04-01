#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"

/**
 * Abstract base class for all 3D shapes
 * Provides common functionality and interface for different geometric objects
 */
class Shape
{
protected:
    // Mesh data
    std::vector<float> m_Vertices;       // Positions, normals, etc.
    std::vector<unsigned int> m_Indices; // Indices for drawing

    // Rendering resources
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;

    // Transformation properties
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    // Rendering settings
    bool m_WireframeMode;

    // Setup OpenGL resources after updating vertices/indices
    void SetupMesh();

public:
    // Constructor with default values
    Shape();

    // Virtual destructor
    virtual ~Shape();

    // Pure virtual methods to be implemented by derived classes
    virtual void Generate() = 0;         // Generate mesh data
    virtual void Update() = 0;           // Update mesh after parameter changes

    // Transformation methods
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    glm::mat4 GetModelMatrix() const;

    // Rendering settings
    void ToggleWireframe();
    bool IsWireframe() const;

    // Get mesh data for rendering
    const std::vector<float>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;
    unsigned int GetVertexCount() const;
    unsigned int GetIndexCount() const;
    
    glm::vec3 GetPosition() const { return m_Position; }
    // Bind for rendering
    void Bind() const;
    void Unbind() const;

    // Draw the shape
    void Draw(Shader& shader, const glm::mat4& view, const glm::mat4& projection) const;
};