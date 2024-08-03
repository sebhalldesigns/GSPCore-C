#include "gsp_window_win32.h"
#include "gsp_window_internal.h"


#include <gsp_debug/gsp_debug.h>

#include <stdio.h>

static gwindow_win32_state_t state;
const static wchar_t CLASS_NAME[] = L"gsp_window";

// implemented at end of file
LRESULT CALLBACK gsp_window_win32_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

gwindow_t gsp_window_win32_create_window() {

    if (state.instance == NULL) {

        gsp_debug_log(INFO, "Starting Win32..");

        state.instance = GetModuleHandle(NULL);
        
        WNDCLASS wc = { 0 };
            wc.lpfnWndProc   = gsp_window_win32_wndproc;
            wc.hInstance     = state.instance;
            wc.lpszClassName = CLASS_NAME;

            if (RegisterClass(&wc) == NULL) {
                DWORD errorCode = GetLastError();

                gsp_debug_log(ERROR, "Failed to register a Win32 class, error code: %lu", errorCode);

                return (gwindow_t) 0;
            }

            gsp_debug_log(INFO, "Started GWin32WindowManager ok");

    }

    HWND win32_window = CreateWindow(
        CLASS_NAME,
        L"Sample Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    ShowWindow(win32_window, SW_SHOW);

    return (gwindow_t) win32_window;
}

void gsp_window_win32_poll_events() {
    while (GetMessage(&state.msg, NULL, 0, 0)) {
        TranslateMessage(&state.msg);
        DispatchMessage(&state.msg);
    }
}

void gsp_window_win32_set_title(gwindow_t window, gstring_t title) {
    SetWindowText((HWND)window, title);
}

LRESULT CALLBACK gsp_window_win32_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    gwindow_event_t event;

    switch (msg) {
        case WM_SIZE:
            float width = (float)LOWORD(lParam);
            float height = (float)HIWORD(lParam);

            uint64_t width_u64 = (uint64_t) width;
            uint64_t height_u64 = (uint64_t) height;
            height_u64 = height_u64 << 32;

            event.event_id = WINDOW_EVENT_RESIZE;
            event.data = width_u64 | height_u64;
            gsp_window_system_event_callback((gwindow_t) hwnd, event);
            break;
        case WM_CLOSE:
            event.event_id = WINDOW_EVENT_CLOSE_REQUEST;
            gsp_window_system_event_callback((gwindow_t) hwnd, event);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

