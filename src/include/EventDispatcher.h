#pragma once

#include "Event.h"
#include <functional>

class EventDispatcher {
private:
    Event& m_Event;

public:
    EventDispatcher(Event& event) : m_Event(event) {}

    // Dispatch event to the appropriate handler based on event type
    template<typename T, typename F>
    bool Dispatch(const F& func) {
        // Check if event type matches the template parameter
        if (m_Event.GetType() == T::GetStaticType()) {
            // Call the function with the event cast to the correct type
            m_Event.SetHandled(func(static_cast<T&>(m_Event)));
            return true;
        }
        return false;
    }
};