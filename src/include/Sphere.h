#pragma once

#include "Shape.h"

class Sphere : public Shape
{
private:
    float m_Radius;
    unsigned int m_Sectors;    // Longitude divisions
    unsigned int m_Stacks;     // Latitude divisions
    bool m_FlatShading;        // Flat or smooth shading

public:
    Sphere(float radius = 1.0f, unsigned int sectors = 36, unsigned int stacks = 18);
    ~Sphere() override;

    // Inherited from Shape
    void Generate() override;
    void Update() override;

    // Sphere-specific methods
    void SetRadius(float radius);
    void SetResolution(unsigned int sectors, unsigned int stacks);
    void ToggleFlatShading();
    
    // Generate mesh with flat shading (different normals)
    void GenerateFlatShadedSphere();
    
    // Getters
    float GetRadius() const { return m_Radius; }
    unsigned int GetSectors() const { return m_Sectors; }
    unsigned int GetStacks() const { return m_Stacks; }
    bool IsFlatShading() const { return m_FlatShading; }
};