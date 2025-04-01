#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch),
    m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(2.5f),
    m_MouseSensitivity(0.1f), m_Zoom(45.0f)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;

    if (direction == FORWARD)
        m_Position += m_Front * velocity;
    if (direction == BACKWARD)
        m_Position -= m_Front * velocity;
    if (direction == LEFT)
        m_Position -= m_Right * velocity;
    if (direction == RIGHT)
        m_Position += m_Right * velocity;
    if (direction == UP)
        m_Position += m_Up * velocity;
    if (direction == DOWN)
        m_Position -= m_Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Constrain pitch to avoid screen flipping
    if (constrainPitch)
    {
        m_Pitch = std::max(-89.0f, std::min(89.0f, m_Pitch));
    }

    // Update front, right and up vectors
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    m_Zoom -= yoffset;
    m_Zoom = std::max(1.0f, std::min(45.0f, m_Zoom));
}

void Camera::UpdateCameraVectors()
{
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    // Recalculate the right and up vectors
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}