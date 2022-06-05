/**
 * @file input.h
 * @author Travis Vroman (travis@kohiengine.com)
 * @brief This file contains everything having to do with input on deskop
 * environments from keyboards and mice. Gamepads and touch controls will
 * likely be handled separately at a future date.
 * @version 1.0
 * @date 2022-01-10
 * 
 * @copyright Kohi Game Engine is Copyright (c) Travis Vroman 2021-2022
 * 
 */
#pragma once

#include "defines.h"

/**
 * @brief Represents available mouse buttons.
 */
typedef enum buttons {
    /** @brief The left mouse button */
    BUTTON_LEFT,
    /** @brief The right mouse button */
    BUTTON_RIGHT,
    /** @brief The middle mouse button (typically the wheel) */
    BUTTON_MIDDLE,
    BUTTON_MAX_BUTTONS
} buttons;

/**
 * @brief Represents available keyboard keys.
 */
typedef enum keys {
    /** @brief The backspace key. */
    KEY_BACKSPACE = 0x00,
    /** @brief The enter key. */
    KEY_ENTER = 0x01,
    /** @brief The tab key. */
    KEY_TAB = 0x02,
    /** @brief The shift key. */
    KEY_SHIFT = 0x03,
    /** @brief The Control/Ctrl key. */
    KEY_CONTROL = 0x04,

    /** @brief The pause key. */
    KEY_PAUSE = 0x05,
    /** @brief The Caps Lock key. */
    KEY_CAPITAL = 0x06,

    /** @brief The Escape key. */
    KEY_ESCAPE = 0x07,

    KEY_CONVERT = 0x08,
    KEY_NONCONVERT = 0x09,
    KEY_ACCEPT = 0x0a,
    KEY_MODECHANGE = 0x0b,

    /** @brief The spacebar key. */
    KEY_SPACE = 0x0c,
    /** @brief The prior key. */
    KEY_PRIOR = 0x0d,
    /** @brief The next key. */
    KEY_NEXT = 0x0e,
    /** @brief The end key. */
    KEY_END = 0x0f,
    /** @brief The home key. */
    KEY_HOME = 0x10,
    /** @brief The left arrow key. */
    KEY_LEFT = 0x11,
    /** @brief The up arrow key. */
    KEY_UP = 0x12,
    /** @brief The right arrow key. */
    KEY_RIGHT = 0x13,
    /** @brief The down arrow key. */
    KEY_DOWN = 0x14,
    /** @brief The select key. */
    KEY_SELECT = 0x15,
    /** @brief The print key. */
    KEY_PRINT = 0x16,
    /** @brief The execute key. */
    KEY_EXECUTE = 0x17,
    /** @brief The snapshot key. */
    KEY_SNAPSHOT = 0x18,
    /** @brief The insert key. */
    KEY_INSERT = 0x19,
    /** @brief The delete key. */
    KEY_DELETE = 0x1a,
    /** @brief The help key. */
    KEY_HELP = 0x1b,

    /** @brief The 0 key */
    KEY_0 = 0x1c,
    /** @brief The 1 key */
    KEY_1 = 0x1d,
    /** @brief The 2 key */
    KEY_2 = 0x1e,
    /** @brief The 3 key */
    KEY_3 = 0x1f,
    /** @brief The 4 key */
    KEY_4 = 0x20,
    /** @brief The 5 key */
    KEY_5 = 0x21,
    /** @brief The 6 key */
    KEY_6 = 0x22,
    /** @brief The 7 key */
    KEY_7 = 0x23,
    /** @brief The 8 key */
    KEY_8 = 0x24,
    /** @brief The 9 key */
    KEY_9 = 0x25,

    /** @brief The A key. */
    KEY_A = 0x26,
    /** @brief The B key. */
    KEY_B = 0x27,
    /** @brief The C key. */
    KEY_C = 0x28,
    /** @brief The D key. */
    KEY_D = 0x29,
    /** @brief The E key. */
    KEY_E = 0x2a,
    /** @brief The F key. */
    KEY_F = 0x2b,
    /** @brief The G key. */
    KEY_G = 0x2c,
    /** @brief The H key. */
    KEY_H = 0x2d,
    /** @brief The I key. */
    KEY_I = 0x2e,
    /** @brief The J key. */
    KEY_J = 0x2f,
    /** @brief The K key. */
    KEY_K = 0x30,
    /** @brief The L key. */
    KEY_L = 0x31,
    /** @brief The M key. */
    KEY_M = 0x32,
    /** @brief The N key. */
    KEY_N = 0x33,
    /** @brief The O key. */
    KEY_O = 0x34,
    /** @brief The P key. */
    KEY_P = 0x35,
    /** @brief The Q key. */
    KEY_Q = 0x36,
    /** @brief The R key. */
    KEY_R = 0x37,
    /** @brief The S key. */
    KEY_S = 0x38,
    /** @brief The T key. */
    KEY_T = 0x39,
    /** @brief The U key. */
    KEY_U = 0x3a,
    /** @brief The V key. */
    KEY_V = 0x3b,
    /** @brief The W key. */
    KEY_W = 0x3c,
    /** @brief The X key. */
    KEY_X = 0x3d,
    /** @brief The Y key. */
    KEY_Y = 0x3e,
    /** @brief The Z key. */
    KEY_Z = 0x3f,

    /** @brief The left Windows/Super key. */
    KEY_LWIN = 0x40,
    /** @brief The right Windows/Super key. */
    KEY_RWIN = 0x41,
    KEY_APPS = 0x42,

    /** @brief The sleep key. */
    KEY_SLEEP = 0x43,

    /** @brief The numberpad 0 key. */
    KEY_NUMPAD0 = 0x44,
    /** @brief The numberpad 1 key. */
    KEY_NUMPAD1 = 0x45,
    /** @brief The numberpad 2 key. */
    KEY_NUMPAD2 = 0x46,
    /** @brief The numberpad 3 key. */
    KEY_NUMPAD3 = 0x47,
    /** @brief The numberpad 4 key. */
    KEY_NUMPAD4 = 0x48,
    /** @brief The numberpad 5 key. */
    KEY_NUMPAD5 = 0x49,
    /** @brief The numberpad 6 key. */
    KEY_NUMPAD6 = 0x4a,
    /** @brief The numberpad 7 key. */
    KEY_NUMPAD7 = 0x4b,
    /** @brief The numberpad 8 key. */
    KEY_NUMPAD8 = 0x4c,
    /** @brief The numberpad 9 key. */
    KEY_NUMPAD9 = 0x4d,
    /** @brief The numberpad multiply key. */
    KEY_MULTIPLY = 0x4e,
    /** @brief The numberpad add key. */
    KEY_ADD = 0x4f,
    /** @brief The numberpad separator key. */
    KEY_SEPARATOR = 0x50,
    /** @brief The numberpad subtract key. */
    KEY_SUBTRACT = 0x51,
    /** @brief The numberpad decimal key. */
    KEY_DECIMAL = 0x52,
    /** @brief The numberpad divide key. */
    KEY_DIVIDE = 0x53,

    /** @brief The F1 key. */
    KEY_F1 = 0x54,
    /** @brief The F2 key. */
    KEY_F2 = 0x55,
    /** @brief The F3 key. */
    KEY_F3 = 0x56,
    /** @brief The F4 key. */
    KEY_F4 = 0x57,
    /** @brief The F5 key. */
    KEY_F5 = 0x58,
    /** @brief The F6 key. */
    KEY_F6 = 0x59,
    /** @brief The F7 key. */
    KEY_F7 = 0x5a,
    /** @brief The F8 key. */
    KEY_F8 = 0x5b,
    /** @brief The F9 key. */
    KEY_F9 = 0x5c,
    /** @brief The F10 key. */
    KEY_F10 = 0x5d,
    /** @brief The F11 key. */
    KEY_F11 = 0x5e,
    /** @brief The F12 key. */
    KEY_F12 = 0x5f,
    /** @brief The F13 key. */
    KEY_F13 = 0x60,
    /** @brief The F14 key. */
    KEY_F14 = 0x61,
    /** @brief The F15 key. */
    KEY_F15 = 0x62,
    /** @brief The F16 key. */
    KEY_F16 = 0x63,
    /** @brief The F17 key. */
    KEY_F17 = 0x64,
    /** @brief The F18 key. */
    KEY_F18 = 0x65,
    /** @brief The F19 key. */
    KEY_F19 = 0x66,
    /** @brief The F20 key. */
    KEY_F20 = 0x67,
    /** @brief The F21 key. */
    KEY_F21 = 0x68,
    /** @brief The F22 key. */
    KEY_F22 = 0x69,
    /** @brief The F23 key. */
    KEY_F23 = 0x6a,
    /** @brief The F24 key. */
    KEY_F24 = 0x6b,

    /** @brief The number lock key. */
    KEY_NUMLOCK = 0x6b,

    /** @brief The scroll lock key. */
    KEY_SCROLL = 0x6d,

    /** @brief The numberpad equal key. */
    KEY_NUMPAD_EQUAL = 0x6e,

    /** @brief The left shift key. */
    KEY_LSHIFT = 0x6f,
    /** @brief The right shift key. */
    KEY_RSHIFT = 0x70,
    /** @brief The left control key. */
    KEY_LCONTROL = 0x71,
    /** @brief The right control key. */
    KEY_RCONTROL = 0x72,
    /** @brief The left alt key. */
    KEY_LALT = 0x73,
    /** @brief The right alt key. */
    KEY_RALT = 0x74,

    /** @brief The semicolon key. */
    KEY_SEMICOLON = 0x75,
    /** @brief The plus key. */
    KEY_PLUS = 0x76,
    /** @brief The comma key. */
    KEY_COMMA = 0x77,
    /** @brief The minus key. */
    KEY_MINUS = 0x78,
    /** @brief The period key. */
    KEY_PERIOD = 0x79,
    /** @brief The slash key. */
    KEY_SLASH = 0x7a,

    /** @brief The grave key. */
    KEY_GRAVE = 0x7b,

    KEYS_MAX_KEYS = 0x7c
} keys;

/**
 * @brief Initializes the input system. Call twice; once to obtain memory requirement (passing
 * state = 0), then a second time passing allocated memory to state.
 * 
 * @param memory_requirement The required size of the state memory.
 * @param state Either 0 or the allocated block of state memory.
 */
void input_system_initialize(u64* memory_requirement, void* state);

/**
 * @brief Shuts the input system down.
 * @param state A pointer to the system state.
 */
void input_system_shutdown(void* state);

/**
 * @brief Updates the input system every frame.
 * @param delta_time The delta time in seconds since the last frame.
 */
void input_update(f64 delta_time);

// keyboard input

/**
 * @brief Indicates if the given key is currently pressed down.
 * @param key They key to be checked.
 * @returns True if currently pressed; otherwise false.
 */
KAPI b8 input_is_key_down(keys key);

/**
 * @brief Indicates if the given key is NOT currently pressed down.
 * @param key They key to be checked.
 * @returns True if currently released; otherwise false.
 */
KAPI b8 input_is_key_up(keys key);

/**
 * @brief Indicates if the given key was previously pressed down on the last frame.
 * @param key They key to be checked.
 * @returns True if was previously pressed; otherwise false.
 */
KAPI b8 input_was_key_down(keys key);

/**
 * @brief Indicates if the given key was previously pressed down in the last frame.
 * @param key They key to be checked.
 * @returns True if previously released; otherwise false.
 */
KAPI b8 input_was_key_up(keys key);

/**
 * @brief Sets the state for the given key.
 * @param key The key to be processed.
 * @param pressed Indicates whether the key is currently pressed.
 */
void input_process_key(keys key, b8 pressed);

// mouse input

/**
 * @brief Indicates if the given mouse button is currently pressed.
 * @param button The button to check.
 * @returns True if currently pressed; otherwise false.
 */
KAPI b8 input_is_button_down(buttons button);

/**
 * @brief Indicates if the given mouse button is currently released.
 * @param button The button to check.
 * @returns True if currently released; otherwise false.
 */
KAPI b8 input_is_button_up(buttons button);

/**
 * @brief Indicates if the given mouse button was previously pressed in the last frame.
 * @param button The button to check.
 * @returns True if previously pressed; otherwise false.
 */
KAPI b8 input_was_button_down(buttons button);

/**
 * @brief Indicates if the given mouse button was previously released in the last frame.
 * @param button The button to check.
 * @returns True if previously released; otherwise false.
 */
KAPI b8 input_was_button_up(buttons button);

/** 
 * @brief Obtains the current mouse position.
 * @param x A pointer to hold the current mouse position on the x-axis.
 * @param y A pointer to hold the current mouse position on the y-axis.
 */
KAPI void input_get_mouse_position(i32* x, i32* y);

/** 
 * @brief Obtains the previous mouse position.
 * @param x A pointer to hold the previous mouse position on the x-axis.
 * @param y A pointer to hold the previous mouse position on the y-axis.
 */
KAPI void input_get_previous_mouse_position(i32* x, i32* y);

/**
 * @brief Sets the press state of the given mouse button.
 * @param button The mouse button whose state to set.
 * @param pressed Indicates if the mouse button is currently pressed.
 */
void input_process_button(buttons button, b8 pressed);

/**
 * @brief Sets the current position of the mouse to the given x and y positions.
 * Also updates the previous position beforehand.
 */
void input_process_mouse_move(i16 x, i16 y);

/**
 * @brief Processes mouse wheel scrolling.
 * @param z_delta The amount of scrolling which occurred on the z axis (mouse wheel)
 */
void input_process_mouse_wheel(i8 z_delta);
