#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

class InputManager {
private:
    static InputManager* s_Instance;

    std::unordered_map<int, bool> m_KeyStates;
    std::unordered_map<int, bool> m_MouseButtonStates;
    glm::vec2 m_MousePosition;
    glm::vec2 m_MouseScrollOffset;

public:
    static InputManager& Get() {
        if (!s_Instance)
            s_Instance = new InputManager();
        return *s_Instance;
    }

    // Key state methods
    bool IsKeyPressed(int keyCode);
    void SetKeyState(int keyCode, bool pressed);

    // Mouse state methods
    bool IsMouseButtonPressed(int button);
    void SetMouseButtonState(int button, bool pressed);
    const glm::vec2& GetMousePosition() const;
    void SetMousePosition(float x, float y);

    // Update method to reset per-frame states
    void Update();
}; #pragma once
