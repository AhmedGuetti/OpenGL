#pragma once
#include <glm/glm.hpp>

class Ray
{
private:
    glm::vec3 m_Origin;
    glm::vec3 m_Direction;

public:
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : m_Origin(origin), m_Direction(glm::normalize(direction))
    {
    }

    glm::vec3 GetOrigin() const { return m_Origin; }
    glm::vec3 GetDirection() const { return m_Direction; }

    // Compute point along ray at distance t
    glm::vec3 GetPointAt(float t) const { return m_Origin + t * m_Direction; }
};