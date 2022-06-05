#include "platform.h"

// Linux platform layer.
#if KPLATFORM_LINUX

#include "core/logger.h"
#include "core/event.h"
#include "core/input.h"

#include "containers/darray.h"

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xkb.h>
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>  // nanosleep
#else
#include <unistd.h>  // usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// For surface creation
#define VK_USE_PLATFORM_XCB_KHR
#include <vulkan/vulkan.h>
#include "renderer/vulkan/vulkan_types.inl"

static const u32 key_lookup_table[KEYS_MAX_KEYS] = {
        /** @brief The backspace key. */
        0xff08,
        /** @brief The enter key. */
        0xff0d,
        /** @brief The tab key. */
        0xff09,
        /** @brief The shift key. */
        KEYS_MAX_KEYS,
        /** @brief The Control/Ctrl key. */
        KEYS_MAX_KEYS,

        /** @brief The pause key. */
        0xff13,
        /** @brief The Caps Lock key. */
        0xffe5,

        /** @brief The Escape key. */
        0xff1b,

        KEYS_MAX_KEYS,
        KEYS_MAX_KEYS,
        KEYS_MAX_KEYS,
        KEYS_MAX_KEYS,

        /** @brief The spacebar key. */
        0xff7e,
        /** @brief The prior key. */
        0xff55,
        /** @brief The next key. */
        0xff56,
        /** @brief The end key. */
        0xff57,
        /** @brief The home key. */
        0xff50,
        /** @brief The left arrow key. */
        0xff51,
        /** @brief The up arrow key. */
        0xff52,
        /** @brief The right arrow key. */
        0xff53,
        /** @brief The down arrow key. */
        0xff54,
        /** @brief The select key. */
        0xff60,
        /** @brief The print key. */
        0xff61,
        /** @brief The execute key. */
        0xff62,
        /** @brief The snapshot key. */
        KEYS_MAX_KEYS,
        /** @brief The insert key. */
        0xff63,
        /** @brief The delete key. */
        0xffff,
        /** @brief The help key. */
        0xff6A,

        /** @brief The 0 key */
        0x0030,
        /** @brief The 1 key */
        0x0031,
        /** @brief The 2 key */
        0x0032,
        /** @brief The 3 key */
        0x0033,
        /** @brief The 4 key */
        0x0034,
        /** @brief The 5 key */
        0x0035,
        /** @brief The 6 key */
        0x0036,
        /** @brief The 7 key */
        0x0037,
        /** @brief The 8 key */
        0x0038,
        /** @brief The 9 key */
        0x0039,

        /** @brief The A key. */
        0x0041,
        /** @brief The B key. */
        0x0042,
        /** @brief The C key. */
        0x0043,
        /** @brief The D key. */
        0x0044,
        /** @brief The E key. */
        0x0045,
        /** @brief The F key. */
        0x0046,
        /** @brief The G key. */
        0x0047,
        /** @brief The H key. */
        0x0048,
        /** @brief The I key. */
        0x0049,
        /** @brief The J key. */
        0x004A,
        /** @brief The K key. */
        0x004B,
        /** @brief The L key. */
        0x004C,
        /** @brief The M key. */
        0x004D,
        /** @brief The N key. */
        0x004E,
        /** @brief The O key. */
        0x004F,
        /** @brief The P key. */
        0x0050,
        /** @brief The Q key. */
        0x0051,
        /** @brief The R key. */
        0x0052,
        /** @brief The S key. */
        0x0053,
        /** @brief The T key. */
        0x0054,
        /** @brief The U key. */
        0x0055,
        /** @brief The V key. */
        0x0056,
        /** @brief The W key. */
        0x0057,
        /** @brief The X key. */
        0x0058,
        /** @brief The Y key. */
        0x0059,
        /** @brief The Z key. */
        0x005A,

        /** @brief The left Windows/Super key. */
        0xffeb,
        /** @brief The right Windows/Super key. */
        0xffec,
        KEYS_MAX_KEYS,

        /** @brief The sleep key. */
        KEYS_MAX_KEYS,

        /** @brief The numberpad 0 key. */
        0xff9e,
        /** @brief The numberpad 1 key. */
        0xff9c,
        /** @brief The numberpad 2 key. */
        0xff99,
        /** @brief The numberpad 3 key. */
        0xff9b,
        /** @brief The numberpad 4 key. */
        0xff96,
        /** @brief The numberpad 5 key. */
        0xff9d,
        /** @brief The numberpad 6 key. */
        0xff98,
        /** @brief The numberpad 7 key. */
        0xff95,
        /** @brief The numberpad 8 key. */
        0xff97,
        /** @brief The numberpad 9 key. */
        0xff9a,
        /** @brief The numberpad multiply key. */
        0xffaa,
        /** @brief The numberpad add key. */
        0xffab,
        /** @brief The numberpad separator key. */
        0xffac,
        /** @brief The numberpad subtract key. */
        0xffad,
        /** @brief The numberpad decimal key. */
        0xff9f,
        /** @brief The numberpad divide key. */
        0xffaf,

        /** @brief The F1 key. */
        0xffbe,
        /** @brief The F2 key. */
        0xffbf,
        /** @brief The F3 key. */
        0xffc0,
        /** @brief The F4 key. */
        0xffc1,
        /** @brief The F5 key. */
        0xffc2,
        /** @brief The F6 key. */
        0xffc3,
        /** @brief The F7 key. */
        0xffc4,
        /** @brief The F8 key. */
        0xffc5,
        /** @brief The F9 key. */
        0xffc6,
        /** @brief The F10 key. */
        0xffc7,
        /** @brief The F11 key. */
        0xffc8,
        /** @brief The F12 key. */
        0xffc9,
        /** @brief The F13 key. */
        0xffca,
        /** @brief The F14 key. */
        0xffcb,
        /** @brief The F15 key. */
        0xffcc,
        /** @brief The F16 key. */
        0xffcd,
        /** @brief The F17 key. */
        0xffce,
        /** @brief The F18 key. */
        0xffcf,
        /** @brief The F19 key. */
        0xffd0,
        /** @brief The F20 key. */
        0xffd1,
        /** @brief The F21 key. */
        0xffd2,
        /** @brief The F22 key. */
        0xffd3,
        /** @brief The F23 key. */
        0xffd4,
        /** @brief The F24 key. */
        0xffd5,

        /** @brief The number lock key. */
        0xff7f,

        /** @brief The scroll lock key. */
        0xff14,

        /** @brief The numberpad equal key. */
        0xffbd,

        /** @brief The left shift key. */
        0xffe1,
        /** @brief The right shift key. */
        0xffe2,
        /** @brief The left control key. */
        0xffe3,
        /** @brief The right control key. */
        0xffe4,
        /** @brief The left alt key. */
        0xffe9,
        /** @brief The right alt key. */
        0xfe03,

        /** @brief The semicolon key. */
        0x003b,
        /** @brief The plus key. */
        0x002b,
        /** @brief The comma key. */
        0x002c,
        /** @brief The minus key. */
        0x002d,
        /** @brief The period key. */
        0x002e,
        /** @brief The slash key. */
        0x002f,

        /** @brief The grave key. */
        0x0060
};

typedef struct platform_state {
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
    xcb_key_symbols_t  *syms;
    VkSurfaceKHR surface;
} platform_state;

static platform_state* state_ptr;

b8 internal_poll_for_event(xcb_generic_event_t **event);
// Key translation
keys translate_keycode(u32 x_keycode);

b8 platform_system_startup(
    u64* memory_requirement,
    void* state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height) {
    *memory_requirement = sizeof(platform_state);
    if (state == 0) {
        return true;
    }

    state_ptr = state;

    // We get the connection through xcb
    int screenp = 0;
    state_ptr->connection = xcb_connect(0, &screenp);
    if (xcb_connection_has_error(state_ptr->connection)) {
        KFATAL("Failed to connect to X server via XCB.");
        return false;
    }

    // Unlike most reply_t this one must not be freed.
    const xcb_query_extension_reply_t  *ext_reply = xcb_get_extension_data(state_ptr->connection, &xcb_xkb_id);
    if (!ext_reply) {
        KFATAL("XKB extension not available on host X11 server.");
        return false;
    }

    // We can now load xcb's extensions (xkb)
    xcb_generic_error_t *error;
    xcb_xkb_use_extension_cookie_t use_ext_cookie;
    xcb_xkb_use_extension_reply_t *use_ext_reply;
    use_ext_cookie = xcb_xkb_use_extension(state_ptr->connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);
    use_ext_reply = xcb_xkb_use_extension_reply(state_ptr->connection, use_ext_cookie, &error);
    if (!use_ext_reply) {
        KFATAL("Couldn't load the xcb-xkb extension.");
        free(use_ext_reply);
        return false;
    }
    if (!use_ext_reply->supported) {
        KFATAL("The XKB extension is not supported on this X server.");
        free(use_ext_reply);
        return false;
    }
    free(use_ext_reply);

    // We can now deactivate repeat for this app only without affecting the system
    xcb_xkb_per_client_flags_cookie_t pcf_cookie;
    xcb_xkb_per_client_flags_reply_t *pcf_reply;
    pcf_cookie = xcb_xkb_per_client_flags(
            state_ptr->connection,
            XCB_XKB_ID_USE_CORE_KBD,
            XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT,
            XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT,
            0, 0, 0);
    pcf_reply = xcb_xkb_per_client_flags_reply(state_ptr->connection, pcf_cookie, &error);
    free(pcf_reply);
    if (error) {
        KERROR("Failed to set XKB per-client flags, not using detectable repeat. error code: %u", error->major_code);
        return false;
    }

    // Now let's grab the keysyms we will use later
    state_ptr->syms = xcb_key_symbols_alloc(state_ptr->connection);

    // Get data from the X server
    const struct xcb_setup_t* setup = xcb_get_setup(state_ptr->connection);

    state_ptr->screen = xcb_setup_roots_iterator(setup).data;

    // Allocate a XID for the window to be created.
    state_ptr->window = xcb_generate_id(state_ptr->connection);

    // Register event types.
    // XCB_CW_BACK_PIXEL = filling then window bg with a single colour
    // XCB_CW_EVENT_MASK is required.
    u32 event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;

    // Listen for keyboard and mouse buttons
    u32 event_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                       XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    // Values to be sent over XCB (bg colour, events)
    u32 value_list[] = {state_ptr->screen->black_pixel, event_values};

    // Create the window
    xcb_create_window(
        state_ptr->connection,
        XCB_COPY_FROM_PARENT,  // depth
        state_ptr->window,
        state_ptr->screen->root,        // parent
        (i16)x,                              //x
        (i16)y,                              //y
        width,                          //width
        height,                         //height
        0,                              // No border
        XCB_WINDOW_CLASS_INPUT_OUTPUT,  //class
        state_ptr->screen->root_visual,
        event_mask,
        value_list);

    // Change the title
    xcb_change_property(
        state_ptr->connection,
        XCB_PROP_MODE_REPLACE,
        state_ptr->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,  // data should be viewed 8 bits at a time
        strlen(application_name),
        application_name);

    // Tell the server to notify when the window manager
    // attempts to destroy the window.
    xcb_intern_atom_cookie_t wm_cookie = xcb_intern_atom(
        state_ptr->connection,
        0,
        strlen("WM_DELETE_WINDOW"),
        "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t* wm_reply = xcb_intern_atom_reply(
        state_ptr->connection,
        wm_cookie,
        NULL);
    state_ptr->wm_delete_win = wm_reply->atom;
    free(wm_reply);
    wm_cookie = xcb_intern_atom(
        state_ptr->connection,
        0,
        strlen("WM_PROTOCOLS"),
        "WM_PROTOCOLS");
    wm_reply = xcb_intern_atom_reply(
        state_ptr->connection,
        wm_cookie,
        NULL);
    state_ptr->wm_protocols = wm_reply->atom;
    free(wm_reply);

    xcb_change_property(
        state_ptr->connection,
        XCB_PROP_MODE_REPLACE,
        state_ptr->window,
        state_ptr->wm_protocols,
        4,
        32,
        1,
        &state_ptr->wm_delete_win);

    // Map the window to the screen
    xcb_map_window(state_ptr->connection, state_ptr->window);

    // Flush the stream
    i32 stream_result = xcb_flush(state_ptr->connection);
    if (stream_result <= 0) {
        KFATAL("An error occurred when flusing the stream: %d", stream_result);
        return false;
    }

    return true;
}

void platform_system_shutdown(void* plat_state) {
    if (state_ptr) {
        xcb_destroy_window(state_ptr->connection, state_ptr->window);
        xcb_key_symbols_free(state_ptr->syms);
        xcb_disconnect(state_ptr->connection);
    }
}

b8 platform_pump_messages() {
    if (state_ptr) {
        xcb_generic_event_t* event;
        xcb_client_message_event_t* cm;

        b8 quit_flagged = false;

        // Poll for events until false is returned.
        while (internal_poll_for_event(&event)) {
            // Input events
            switch (event->response_type & ~0x80) {
                case XCB_KEY_PRESS: {
                    xcb_key_press_event_t *key_event = (xcb_key_press_event_t *)event;
                    xcb_keysym_t key_sym = xcb_key_symbols_get_keysym(state_ptr->syms, key_event->detail, 0);
                    input_process_key(translate_keycode(key_sym), true);
                } break;
                case XCB_KEY_RELEASE: {
                    xcb_key_release_event_t *key_event = (xcb_key_release_event_t *)event;
                    xcb_keysym_t key_sym = xcb_key_symbols_get_keysym(state_ptr->syms, key_event->detail, 0);
                    input_process_key(translate_keycode(key_sym), false);
                } break;
                // we need to separate the PRESS and RELEASE events to handle the WHEEL event
                case XCB_BUTTON_PRESS: {
                    xcb_button_press_event_t *button_event = (xcb_button_press_event_t *)event;
                    // the wheel event is mapped to button 4 and 5
                    // 4 is down, while 5 is up
                    if (button_event->detail > 3) {
                        input_process_mouse_wheel(button_event->detail == 4 ? -1 : 1);
                    } else {
                        input_process_button(button_event->detail, true);
                    }
                }
                case XCB_BUTTON_RELEASE: {
                    xcb_button_press_event_t *button_event = (xcb_button_press_event_t *)event;
                    input_process_button(button_event->detail, false);
                } break;
                case XCB_MOTION_NOTIFY: {
                    // Mouse move
                    xcb_motion_notify_event_t* move_event = (xcb_motion_notify_event_t*)event;

                    // Pass over to the input subsystem.
                    input_process_mouse_move(move_event->event_x, move_event->event_y);
                } break;
                case XCB_CONFIGURE_NOTIFY: {
                    // Resizing - note that this is also triggered by moving the window, but should be
                    // passed anyway since a change in the x/y could mean an upper-left resize.
                    // The application layer can decide what to do with this.
                    xcb_configure_notify_event_t* configure_event = (xcb_configure_notify_event_t*)event;

                    // Fire the event. The application layer should pick this up, but not handle it
                    // as it shouldn be visible to other parts of the application.
                    event_context context;
                    context.data.u16[0] = configure_event->width;
                    context.data.u16[1] = configure_event->height;
                    event_fire(EVENT_CODE_RESIZED, 0, context);

                } break;

                case XCB_CLIENT_MESSAGE: {
                    cm = (xcb_client_message_event_t*)event;

                    // Window close
                    if (cm->data.data32[0] == state_ptr->wm_delete_win) {
                        quit_flagged = true;
                    }
                } break;
                default:
                    // Something else
                    break;
            }

            free(event);
        }
        return !quit_flagged;
    }
    return true;
}

void* platform_allocate(u64 size, b8 aligned) {
    return malloc(size);
}
void platform_free(void* block, b8 aligned) {
    free(block);
}
void* platform_zero_memory(void* block, u64 size) {
    return memset(block, 0, size);
}
void* platform_copy_memory(void* dest, const void* source, u64 size) {
    return memcpy(dest, source, size);
}
void* platform_set_memory(void* dest, i32 value, u64 size) {
    return memset(dest, value, size);
}

void platform_console_write(const char* message, u8 colour) {
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colour_strings[colour], message);
}
void platform_console_write_error(const char* message, u8 colour) {
    // FATAL,ERROR,WARN,INFO,DEBUG,TRACE
    const char* colour_strings[] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
    printf("\033[%sm%s\033[0m", colour_strings[colour], message);
}

f64 platform_get_absolute_time() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
}

void platform_sleep(u64 ms) {
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000 * 1000;
    nanosleep(&ts, 0);
#else
    if (ms >= 1000) {
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

void platform_get_required_extension_names(const char*** names_darray) {
    darray_push(*names_darray, &"VK_KHR_xcb_surface");  // VK_KHR_xlib_surface?
}

// Surface creation for Vulkan
b8 platform_create_vulkan_surface(vulkan_context* context) {
    if(!state_ptr) {
        return false;
    }

    VkXcbSurfaceCreateInfoKHR create_info = {VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR};
    create_info.connection = state_ptr->connection;
    create_info.window = state_ptr->window;

    VkResult result = vkCreateXcbSurfaceKHR(
        context->instance,
        &create_info,
        context->allocator,
        &state_ptr->surface);
    if (result != VK_SUCCESS) {
        KFATAL("Vulkan surface creation failed.");
        return false;
    }

    context->surface = state_ptr->surface;
    return true;
}

b8 internal_poll_for_event(xcb_generic_event_t **event) {
    if (state_ptr) {
        *event = xcb_poll_for_event(state_ptr->connection);
    }

    return (*event != NULL);
}

// Key translation
keys translate_keycode(u32 x_keycode) {
    xcb_keysym_t upper = x_keycode;
    if ((x_keycode >> 8) == 0) {
        if (x_keycode >= 0x0061 && x_keycode <= 0x007a) {
            upper -= (0x0061 - 0x0041);
        }
    }
    for (u32 i = 0; i < KEYS_MAX_KEYS; ++i) {
        if (key_lookup_table[i] == upper) {
            return i;
        }
    }

    return 0;
}

#endif