#include "GSPCore/Basic/GWindow.h"

#include "GSPCore/Basic/GApplication.h"
#include "GSPCore/Basic/GVector.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wchar.h>

GWindow GWindow_Init(GWindowInfo info) {

    GWindowDef* window = calloc(1, sizeof(GWindowDef));

    if (window == NULL || app == NULL) {
        return NULL;
    }

    printf("INIT WINDOW\n");

    window->info = info;

    HWND handle = CreateWindow(
        CLASS_NAME,
        L"Sample Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    ShowWindow(handle, SW_SHOW);

    window->rawHandle = handle;

    GApplicationDef* appDef = (GApplicationDef*)app;
    GVector_Add(appDef->windows, window);

    return window;
}


void GWindow_Free(GWindow window) {
    
    if (window == NULL || app == NULL) {
        return NULL;
    }

    GApplicationDef* appDef = (GApplicationDef*)app;
    GVector_Remove(appDef->windows, window);

    free(window);
}


void GWindow_Close(GWindow window) {
    if (window == NULL) {
        return;
    }

    DestroyWindow((HWND)(((GWindowDef*)window)->rawHandle));
    GWindow_Free(window);
}


void GWindow_SetController(GWindow window, GWindowController controller) {
    if (window == NULL) {
        return;
    }

    GWindowDef* windowDef = (GWindowDef*)window;
    windowDef->controller = controller;

}