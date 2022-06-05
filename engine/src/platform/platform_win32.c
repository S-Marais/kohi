#include "platform/platform.h"

// Windows platform layer.
#if KPLATFORM_WINDOWS

#include "core/logger.h"
#include "core/input.h"
#include "core/event.h"

#include "containers/darray.h"

#include <windows.h>
#include <windowsx.h>  // param input extraction
#include <stdlib.h>

// For surface creation
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>
#include "renderer/vulkan/vulkan_types.inl"

typedef struct platform_state {
    HINSTANCE h_instance;
    HWND hwnd;
    VkSurfaceKHR surface;

} platform_state;

static const int key_lookup_table[KEYS_MAX_KEYS] = {
    0x08,
    0x0D,
    /** @brief The tab key. */
    0x09,
    /** @brief The shift key. */
    0x10,
    /** @brief The Control/Ctrl key. */
    0x11,

    /** @brief The pause key. */
    0x13,
    /** @brief The Caps Lock key. */
    0x14,

    /** @brief The Escape key. */
    0x1B,

    0x1C,
    0x1D,
    0x1E,
    0x1F,

    /** @brief The spacebar key. */
    0x20,
    0x21,
    0x22,
    /** @brief The end key. */
    0x23,
    /** @brief The home key. */
    0x24,
    /** @brief The left arrow key. */
    0x25,
    /** @brief The up arrow key. */
    0x26,
    /** @brief The right arrow key. */
    0x27,
    /** @brief The down arrow key. */
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    /** @brief The insert key. */
    0x2D,
    /** @brief The delete key. */
    0x2E,
    0x2F,

    /** @brief The 0 key */
    0x30,
    /** @brief The 1 key */
    0x31,
    /** @brief The 2 key */
    0x32,
    /** @brief The 3 key */
    0x33,
    /** @brief The 4 key */
    0x34,
    /** @brief The 5 key */
    0x35,
    /** @brief The 6 key */
    0x36,
    /** @brief The 7 key */
    0x37,
    /** @brief The 8 key */
    0x38,
    /** @brief The 9 key */
    0x39,

    /** @brief The A key. */
    0x41,
    /** @brief The B key. */
    0x42,
    /** @brief The C key. */
    0x43,
    /** @brief The D key. */
    0x44,
    /** @brief The E key. */
    0x45,
    /** @brief The F key. */
    0x46,
    /** @brief The G key. */
    0x47,
    /** @brief The H key. */
    0x48,
    /** @brief The I key. */
    0x49,
    /** @brief The J key. */
    0x4A,
    /** @brief The K key. */
    0x4B,
    /** @brief The L key. */
    0x4C,
    /** @brief The M key. */
    0x4D,
    /** @brief The N key. */
    0x4E,
    /** @brief The O key. */
    0x4F,
    /** @brief The P key. */
    0x50,
    /** @brief The Q key. */
    0x51,
    /** @brief The R key. */
    0x52,
    /** @brief The S key. */
    0x53,
    /** @brief The T key. */
    0x54,
    /** @brief The U key. */
    0x55,
    /** @brief The V key. */
    0x56,
    /** @brief The W key. */
    0x57,
    /** @brief The X key. */
    0x58,
    /** @brief The Y key. */
    0x59,
    /** @brief The Z key. */
    0x5A,

    /** @brief The left Windows/Super key. */
    0x5B,
    /** @brief The right Windows/Super key. */
    0x5C,
    0x5D,

    /** @brief The sleep key. */
    0x5F,

    /** @brief The numberpad 0 key. */
    0x60,
    /** @brief The numberpad 1 key. */
    0x61,
    /** @brief The numberpad 2 key. */
    0x62,
    /** @brief The numberpad 3 key. */
    0x63,
    /** @brief The numberpad 4 key. */
    0x64,
    /** @brief The numberpad 5 key. */
    0x65,
    /** @brief The numberpad 6 key. */
    0x66,
    /** @brief The numberpad 7 key. */
    0x67,
    /** @brief The numberpad 8 key. */
    0x68,
    /** @brief The numberpad 9 key. */
    0x69,
    /** @brief The numberpad multiply key. */
    0x6A,
    /** @brief The numberpad add key. */
    0x6B,
    /** @brief The numberpad separator key. */
    0x6C,
    /** @brief The numberpad subtract key. */
    0x6D,
    /** @brief The numberpad decimal key. */
    0x6E,
    /** @brief The numberpad divide key. */
    0x6F,

    /** @brief The F1 key. */
    0x70,
    /** @brief The F2 key. */
    0x71,
    /** @brief The F3 key. */
    0x72,
    /** @brief The F4 key. */
    0x73,
    /** @brief The F5 key. */
    0x74,
    /** @brief The F6 key. */
    0x75,
    /** @brief The F7 key. */
    0x76,
    /** @brief The F8 key. */
    0x77,
    /** @brief The F9 key. */
    0x78,
    /** @brief The F10 key. */
    0x79,
    /** @brief The F11 key. */
    0x7A,
    /** @brief The F12 key. */
    0x7B,
    /** @brief The F13 key. */
    0x7C,
    /** @brief The F14 key. */
    0x7D,
    /** @brief The F15 key. */
    0x7E,
    /** @brief The F16 key. */
    0x7F,
    /** @brief The F17 key. */
    0x80,
    /** @brief The F18 key. */
    0x81,
    /** @brief The F19 key. */
    0x82,
    /** @brief The F20 key. */
    0x83,
    /** @brief The F21 key. */
    0x84,
    /** @brief The F22 key. */
    0x85,
    /** @brief The F23 key. */
    0x86,
    /** @brief The F24 key. */
    0x87,

    /** @brief The number lock key. */
    0x90,

    /** @brief The scroll lock key. */
    0x91,

    /** @brief The numberpad equal key. */
    0x92,

    /** @brief The left shift key. */
    0xA0,
    /** @brief The right shift key. */
    0xA1,
    /** @brief The left control key. */
    0xA2,
    /** @brief The right control key. */
    0xA3,
    /** @brief The left alt key. */
    0xA4,
    /** @brief The right alt key. */
    0xA5,

    /** @brief The semicolon key. */
    0xBA,
    /** @brief The plus key. */
    0xBB,
    /** @brief The comma key. */
    0xBC,
    /** @brief The minus key. */
    0xBD,
    /** @brief The period key. */
    0xBE,
    /** @brief The slash key. */
    0xBF,

    /** @brief The grave key. */
    0xC0
};

static platform_state *state_ptr;

// Clock
static f64 clock_frequency;
static LARGE_INTEGER start_time;

keys translate_keycode(u16 key);
LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param);

void clock_setup() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}

b8 platform_system_startup(
    u64 *memory_requirement,
    void *state,
    const char *application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    *memory_requirement = sizeof(platform_state);
    if (state == 0) {
        return true;
    }
    state_ptr = state;
    state_ptr->h_instance = GetModuleHandleA(0);

    // Setup and register window class.
    HICON icon = LoadIcon(state_ptr->h_instance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS;  // Get double-clicks
    wc.lpfnWndProc = win32_process_message;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state_ptr->h_instance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // NULL; // Manage the cursor manually
    wc.hbrBackground = NULL;                   // Transparent
    wc.lpszClassName = "kohi_window_class";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // Create window
    u32 client_x = x;
    u32 client_y = y;
    u32 client_width = width;
    u32 client_height = height;

    u32 window_x = client_x;
    u32 window_y = client_y;
    u32 window_width = client_width;
    u32 window_height = client_height;

    u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 window_ex_style = WS_EX_APPWINDOW;

    window_style |= WS_MAXIMIZEBOX;
    window_style |= WS_MINIMIZEBOX;
    window_style |= WS_THICKFRAME;

    // Obtain the size of the border.
    RECT border_rect = {0, 0, 0, 0};
    AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);

    // In this case, the border rectangle is negative.
    window_x += border_rect.left;
    window_y += border_rect.top;

    // Grow by the size of the OS border.
    window_width += border_rect.right - border_rect.left;
    window_height += border_rect.bottom - border_rect.top;

    HWND handle = CreateWindowExA(
        window_ex_style, "kohi_window_class", application_name,
        window_style, window_x, window_y, window_width, window_height,
        0, 0, state_ptr->h_instance, 0);

    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        KFATAL("Window creation failed!");
        return false;
    } else {
        state_ptr->hwnd = handle;
    }

    // Show the window
    b32 should_activate = 1;  // TODO: if the window should not accept input, this should be false.
    i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
    // If initially minimized, use SW_MINIMIZE : SW_SHOWMINNOACTIVE;
    // If initially maximized, use SW_SHOWMAXIMIZED : SW_MAXIMIZE
    ShowWindow(state_ptr->hwnd, show_window_command_flags);

    // Clock setup
    clock_setup();

    return true;
}

void platform_system_shutdown(void *plat_state) {
    if (state_ptr && state_ptr->hwnd) {
        DestroyWindow(state_ptr->hwnd);
        state_ptr->hwnd = 0;
    }
}

b8 platform_pump_messages() {
    if (state_ptr) {
        MSG message;
        while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
    return true;
}

void *platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}

void platform_free(void *block, b8 aligned) {
    free(block);
}

void *platform_zero_memory(void *block, u64 size) {
    return memset(block, 0, size);
}

void *platform_copy_memory(void *dest, const void *source, u64 size) {
    return memcpy(dest, source, size);
}

void *platform_set_memory(void *dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char *message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), message, (DWORD)length, number_written, 0);
}

void platform_console_write_error(const char *message, u8 colour) {
    HANDLE console_handle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(console_handle, levels[colour]);
    OutputDebugStringA(message);
    u64 length = strlen(message);
    LPDWORD number_written = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), message, (DWORD)length, number_written, 0);
}

f64 platform_get_absolute_time() {
    if (!clock_frequency) {
        clock_setup();
    }

    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
    Sleep(ms);
}

void platform_get_required_extension_names(const char ***names_darray) {
    darray_push(*names_darray, &"VK_KHR_win32_surface");
}

// Surface creation for Vulkan
b8 platform_create_vulkan_surface(vulkan_context *context) {
    if (!state_ptr) {
        return false;
    }
    VkWin32SurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
    create_info.hinstance = state_ptr->h_instance;
    create_info.hwnd = state_ptr->hwnd;

    VkResult result = vkCreateWin32SurfaceKHR(context->instance, &create_info, context->allocator, &state_ptr->surface);
    if (result != VK_SUCCESS) {
        KFATAL("Vulkan surface creation failed.");
        return false;
    }

    context->surface = state_ptr->surface;
    return true;
}

LRESULT CALLBACK win32_process_message(HWND hwnd, u32 msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_ERASEBKGND:
            // Notify the OS that erasing will be handled by the application to prevent flicker.
            return 1;
        case WM_CLOSE:
            // TODO: Fire an event for the application to quit.
            event_context data = {};
            event_fire(EVENT_CODE_APPLICATION_QUIT, 0, data);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            // Get the updated size.
            RECT r;
            GetClientRect(hwnd, &r);
            u32 width = r.right - r.left;
            u32 height = r.bottom - r.top;

            // Fire the event. The application layer should pick this up, but not handle it
            // as it shouldn be visible to other parts of the application.
            event_context context;
            context.data.u16[0] = (u16)width;
            context.data.u16[1] = (u16)height;
            event_fire(EVENT_CODE_RESIZED, 0, context);
        } break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            // Key pressed/released
            b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            u16 key = (u16)w_param;

            // Check for extended scan code.
            b8 is_extended = (HIWORD(l_param) & KF_EXTENDED) == KF_EXTENDED;

            // Keypress only determines if _any_ alt/ctrl/shift key is pressed. Determine which one if so.
            if (w_param == VK_MENU) {
                key = is_extended ? 0xa5 : 0xa4;
            } else if (w_param == VK_SHIFT) {
                // Annoyingly, KF_EXTENDED is not set for shift keys.
                u32 left_shift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
                u32 scancode = ((l_param & (0xFF << 16)) >> 16);
                key = scancode == left_shift ? 0xa0 : 0xa1;
            } else if (w_param == VK_CONTROL) {
                key = is_extended ? 0xa3 : 0xa2;
            }

            // Pass to the input subsystem for processing.
            input_process_key(translate_keycode(key), pressed);

            // Return 0 to prevent default window behaviour for some keypresses, such as alt.
            return 0;
        }
        case WM_MOUSEMOVE: {
            // Mouse move
            i32 x_position = GET_X_LPARAM(l_param);
            i32 y_position = GET_Y_LPARAM(l_param);

            // Pass over to the input subsystem.
            input_process_mouse_move(x_position, y_position);
        } break;
        case WM_MOUSEWHEEL: {
            i32 z_delta = GET_WHEEL_DELTA_WPARAM(w_param);
            if (z_delta != 0) {
                // Flatten the input to an OS-independent (-1, 1)
                z_delta = (z_delta < 0) ? -1 : 1;
                input_process_mouse_wheel(z_delta);
            }
        } break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            b8 pressed = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN;
            buttons mouse_button = BUTTON_MAX_BUTTONS;
            switch (msg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    mouse_button = BUTTON_LEFT;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    mouse_button = BUTTON_MIDDLE;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    mouse_button = BUTTON_RIGHT;
                    break;
            }

            // Pass over to the input subsystem.
            if (mouse_button != BUTTON_MAX_BUTTONS) {
                input_process_button(mouse_button, pressed);
            }
        } break;
    }

    return DefWindowProcA(hwnd, msg, w_param, l_param);
}

keys translate_keycode(u16 key) {
    for (u16 i = 0; i < KEYS_MAX_KEYS; ++i) {
        if (key_lookup_table[i] == key) {
            return i;
        }
    }
}

#endif  // KPLATFORM_WINDOWS
