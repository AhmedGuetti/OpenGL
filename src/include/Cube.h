#pragma once

#include "Shape.h"

class Cube : public Shape
{
private:
    float m_Width;
    float m_Height;
    float m_Depth;
    glm::vec4 m_Color;

public:
    Cube(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    ~Cube() override;

    // Inherited from Shape
    void Generate() override;
    void Update() override;

    // Cube-specific methods
    void SetDimensions(float width, float height, float depth);
    void SetColor(const glm::vec4& color);

    // Getters
    float GetWidth() const { return m_Width; }
    float GetHeight() const { return m_Height; }
    float GetDepth() const { return m_Depth; }
    glm::vec4 GetColor() const { return m_Color; }
};