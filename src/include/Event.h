#pragma once

#include <string>


enum class EventType {
    None,
    WindowResize, WindowClose, WindowFocus,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};


class Event {
protected:
    bool m_Handled = false;

public:
    virtual ~Event() = default;

    virtual EventType GetType() const = 0;
    virtual std::string GetName() const = 0;

    bool IsHandled() const { return m_Handled; }
    void SetHandled(bool handled) { m_Handled = handled; }
};