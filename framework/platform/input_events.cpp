#include "input_events.h"

namespace clan
{
    InputEvent::InputEvent(EventSource source) :
        m_source{source}
    {}

    EventSource InputEvent::get_source() const
    {
        return m_source;
    }

    KeyInputEvent::KeyInputEvent(KeyCode code, KeyAction action) :
        InputEvent{EventSource::Keyboard},
        m_code{code},
        m_action{action}
    {}

    KeyCode KeyInputEvent::get_code() const
    {
        return m_code;
    }

    KeyAction KeyInputEvent::get_action() const
    {
        return m_action;
    }

    MouseButtonInputEvent::MouseButtonInputEvent(MouseButton button, MouseAction action, float pos_x, float pos_y) :
        InputEvent{EventSource::Mouse},
        m_button{button},
        m_action{action},
        m_pos_x{pos_x},
        m_pos_y{pos_y}
    {}

    MouseButton MouseButtonInputEvent::get_button() const
    {
        return m_button;
    }

    MouseAction MouseButtonInputEvent::get_action() const
    {
        return m_action;
    }

    float MouseButtonInputEvent::get_pos_x() const
    {
        return m_pos_x;
    }

    float MouseButtonInputEvent::get_pos_y() const
    {
        return m_pos_y;
    }
}