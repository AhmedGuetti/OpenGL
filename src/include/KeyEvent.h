#pragma once
#include "Event.h"


class KeyEvent : public Event {
protected:
    int m_KeyCode;

    KeyEvent(int keyCode) : m_KeyCode(keyCode) {}

public:
    int GetKeyCode() const { return m_KeyCode; }
};




class KeyPressedEvent : public KeyEvent {
private:
    bool m_IsRepeat;

public:
    KeyPressedEvent(int keyCode, bool isRepeat)
        : KeyEvent(keyCode), m_IsRepeat(isRepeat) {
    }

    bool IsRepeat() const { return m_IsRepeat; }

    EventType GetType() const override { return EventType::KeyPressed; }
    std::string GetName() const override { return "KeyPressed"; }
};



class KeyReleasedEvent : public KeyEvent {
private:
    bool m_IsRepeat;

public:
    KeyReleasedEvent(int keyCode, bool isRepeat)
        : KeyEvent(keyCode), m_IsRepeat(isRepeat) {
    }

    bool IsRepeat() const { return m_IsRepeat; }

    EventType GetType() const override { return EventType::KeyTyped; }
    std::string GetName() const override { return "KeyTyped"; }
};




class KeyTypedEvent : public KeyEvent {
private:
    bool m_IsRepeat;

public:
    KeyTypedEvent(int keyCode, bool isRepeat)
        : KeyEvent(keyCode), m_IsRepeat(isRepeat) {
    }

    bool IsRepeat() const { return m_IsRepeat; }

    EventType GetType() const override { return EventType::KeyTyped; }
    std::string GetName() const override { return "KeyTyped"; }
};
