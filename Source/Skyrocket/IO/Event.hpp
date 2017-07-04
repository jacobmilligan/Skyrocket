//
// Event.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 8/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include "Skyrocket/IO/Keycodes.hpp"
#include "Skyrocket/IO/Buttons.hpp"

namespace sky {

/// @brief Enum representing the possible types of events
enum class EventType {
    /// @brief The null or empty event
    none,
    /// @brief Fired when a window is moved
    window_moved,
    /// @brief Fired when a window is resized
    window_resized,
    /// @brief Fired when a window is closed
    window_closed,
    /// @brief Fired when a window is damaged and requires a refresh
    window_refresh,
    /// @brief Fired when a window gains focus
    window_focus,
    /// @brief Fired when a window loses focus
    window_defocused,
    /// @brief Fired when a window changes from normal to minimized
    window_minimized,
    /// @brief Fired when a window is restored from minimized state to normal
    window_restored,
    /// @brief Fired when the framebuffer is resized
    framebuffer_resized,
    /// @brief Fired when a mouse button is triggered
    mouse_pressed,
    /// @brief Fired when a mouse button was previously pressed and then released
    mouse_released,
    /// @brief Fired when the mouse pointer is moved
    mouse_moved,
    /// @brief Fired when the mouse pointer moves from outside a window to inside
    /// its bounds
    mouse_entered,
    /// @brief Fired when the mouse pointer moves from inside a windows bounds to
    /// outside
    mouse_exit,
    /// @brief Fired when the scroll wheel is used
    scroll,
    /// @brief Fired when a key on the keyboard was in a released state and is then
    /// pressed down
    key_pressed,
    /// @brief Fired when a key was in a pressed state and is remains in a pressed
    /// state in the next frame
    key_held,
    /// @brief Fired when a key was in the pressed or held state and is then
    /// released
    key_released,
    character_input,
//    file_dropped
};

/// @brief Event is a lightweight structure that tracks input events and their
/// related attributes. The structure is essentially a union of all possible event
/// types as multiple events, allowing querying of the structure at runtime for the
/// existence of a particular type of event of interest
struct Event {
    
    /// @brief The result of a framebuffer or window resize event
    struct SizeEvent {
        /// @brief The new width of frame
        int width;
        /// @brief The new height of the frame
        int height;
    };
    
    /// @brief The result of a character input event
    struct CharacterEvent {
        int codepoint;
        ModifierKey modifier;
    };
    
    /// @brief Describes a mouse_pressed or mouse_released event
    struct MouseEvent {
        /// @brief This holds the button pressed/released
        Button button;
        /// @brief Any modifiers pressed or held down while the mouse button
        /// was pressed/released, if any
        ModifierKey modifier;
    };
    
    /// @brief Describes a key_pressed, key_released, or key_held event
    struct KeyEvent {
        /// @brief The key that was held/released/pressed during the event
        Key key;
        /// @brief The keys integer scancode. This is different for different
        /// platforms so can be used for platform-specific event handling
        int scancode;
        /// @brief A modifier key that was held down at the same time as the key
        /// was triggered, if any
        ModifierKey modifier;
    };
    
    struct FileEvent {
        int count;
        char** paths;
    };
    
    /// @brief Describes a mouse_moved, mouse_entered, mouse_exited, mouse_scroll
    /// or window_moved event
    struct PositionEvent {
        /// @brief The new x position of the event subject
        double x;
        /// @brief The new y position of the event subject
        double y;
    };
    
    /// @brief A union of all possible event types. This allows querying of multiple
    /// event types' attributes through the one interface while remaining lightweight
    union {
        /// @brief Describes any window_position events that occurred
        PositionEvent window_position;
        /// @brief Describes any mouse_position events that occurred
        PositionEvent mouse_position;
        /// @brief Describes any mouse_scroll events that occurred
        PositionEvent mouse_scroll;
        /// @brief Describes any sizing events that occurred
        SizeEvent size;
        /// @brief Describes any keyboard-related events that occurred
        KeyEvent keyboard;
        /// @brief Describes any mouse-related events that occurred
        MouseEvent mouse;
        /// @brief Describes any character_input events that occurred
        CharacterEvent character;
        /// @brief Describes any fileio events that occurred
        FileEvent file;
    };
    
    /// @brief The type of event that occurred. This will always be that last event
    /// type held in the event queue
    EventType type;
};


}
