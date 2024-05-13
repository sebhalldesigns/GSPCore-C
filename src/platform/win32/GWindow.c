#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"

#include "GSPCore/GLog.h"
#include "GSPCore/GVector.h"
#include "GSPCore/GRenderManager.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef UNICODE
#define UNICODE
#endif 
#include <Windows.h>
#include <Windowsx.h>

#include <wchar.h>


const wchar_t CLASS_NAME[]  = L"GSPCore Window";
static MSG win32Msg;

// common statics
static GVector windowVector = NULL;


static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
GWindowDef* TryGetWindow(HWND hwnd);
void ButtonDown(GWindowDef* windowDef, LPARAM lParam, uint8_t button);
void ButtonUp(GWindowDef* windowDef, LPARAM lParam, uint8_t button);

bool GWindowDef_Init() {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == NULL) {
        DWORD errorCode = GetLastError();

        DEBUG_LOG(ERROR, "Failed to register a Win32 class, error code: %lu", errorCode);

        return false;
    } else {
        printf("Set up Win32 ok\n");
    }   

    /*
    if (!glewInit()) {
        DEBUG_LOG(ERROR, "Failed to start GLEW");
        return false;
    }*/
    return true;
}


GWindow GWindow_Init(GWindowInfo info) {

    GWindowDef* window = calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

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

    if (handle == NULL) {
        DEBUG_LOG(ERROR, "Failed to create a win32 window.");
        return NULL;
    }
        

    // Initialize OpenGL
    /*HDC hDC;
    HGLRC hRC;
    if (!InitOpenGL(handle, &hDC, &hRC)) {
         DEBUG_LOG(ERROR, "Failed to create an OpenGL context.");
    }*/
        
    
    window->title = info.title;
    window->rawHandle = (uintptr_t)handle;
    window->width = info.width;
    window->height = info.height;
    window->hinstance = GetModuleHandle(NULL);

    GRenderManager_SetupWindow(window);

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);

    if (windowVector == NULL) {
        windowVector = GVector_Init();
    }

    if (windowVector != NULL) {
        GVector_Add(windowVector, (GVectorItem)window);
        DEBUG_LOG(INFO, "Allocated GWindow at %lu", window);
        return window;
    } else {
        DEBUG_LOG(ERROR, "Failed to allocate a window vector.");
        return NULL;
    }
}

void GWindow_SetUserData(GWindow window, void* userData) {
    if (window == NULL) {
        return NULL;
    }

    ((GWindowDef*)window)->userData = userData;

}

void GWindow_SetDrawDelegate(GWindow window, GWindowDrawDelegate drawDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->drawDelegate = drawDelegate;
    }
}

void GWindow_SetCloseDelegate(GWindow window, GWindowCloseDelegate closeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->closeDelegate = closeDelegate;
    }
}
void GWindow_SetWillCloseDelegate(GWindow window, GWindowWillCloseDelegate willCloseDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->willCloseDelegate = willCloseDelegate;
    }
}

void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->resizeDelegate = resizeDelegate;
    }
}

void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->pointerMoveDelegate = pointerMoveDelegate;
    }
}

void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonDownDelegate = buttonDownDelegate;
    }
}

void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonUpDelegate = buttonUpDelegate;
    }
}

void GWindow_Close(GWindow window) {

    if (GVector_Contains(windowVector, window)) {

        DestroyWindow((HWND)(((GWindowDef*)window)->rawHandle));
        GVector_Remove(windowVector, (GVectorItem)window);

        DEBUG_LOG(INFO, "Closed GWindow at %lu", (uintptr_t)window);

    }
}

void GWindowDef_Poll() { 

    BOOL bRet;
    while ((bRet = PeekMessage(&win32Msg, NULL, 0, 0, PM_REMOVE)) != 0) {
        TranslateMessage(&win32Msg);
        DispatchMessage(&win32Msg);
    }

}

size_t GWindowDef_NumberOfOpenWindows() {
    return GVector_Size(windowVector);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {  

    //return DefWindowProc(hwnd, msg, wParam, lParam);

    GWindowDef* windowDef = TryGetWindow(hwnd);

    if (windowDef == NULL) {
        // some events will be raised before the window is registed to the vector, so ignore this
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }



    switch (msg) {
        case WM_PAINT:
          
            GRenderManager_RenderWindow(windowDef);

            break;

        case WM_SIZE:
            int newWidth = LOWORD(lParam);
            int newHeight = HIWORD(lParam);

             if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef->userData, windowDef, newSize);
                }
            }

            break;
        case WM_DISPLAYCHANGE:
            InvalidateRect(hwnd, NULL, true);
            break;
        case WM_MOUSEMOVE:

            GWindowPoint motionLocation = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            if (windowDef->pointerMoveDelegate != NULL) {
                (windowDef->pointerMoveDelegate)(windowDef->userData, windowDef, motionLocation);
            }

            break;
        case WM_LBUTTONDOWN:
            ButtonDown(windowDef, lParam, 1);
            break;
        case WM_MBUTTONDOWN:
            ButtonDown(windowDef, lParam, 2);
            break;
        case WM_RBUTTONDOWN:
            ButtonDown(windowDef, lParam, 3);
            break;
        case WM_LBUTTONUP:
            ButtonUp(windowDef, lParam, 1);
            break;
        case WM_MBUTTONUP:
            ButtonUp(windowDef, lParam, 2);
            break;
        case WM_RBUTTONUP:
            ButtonUp(windowDef, lParam, 3);
            break;    
        case WM_CLOSE:

                if (windowDef->willCloseDelegate != NULL) {

                    if ((windowDef->willCloseDelegate)(windowDef->userData, (GWindow)windowDef)) {
                        if (windowDef->closeDelegate != NULL) {
                            (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                        }

                        GWindow_Close((GWindow)windowDef);
                        return 0;
                    }
                } else {
                    
                    if (windowDef->closeDelegate != NULL) {
                        (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                    }

                    GWindow_Close((GWindow)windowDef);
                    return 0;
                }
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

GWindowDef* TryGetWindow(HWND hwnd) {

    for (int i = 0; i < GVector_Size(windowVector); i++) {
        
        GVectorItem item = GVector_Get(windowVector, i);
        if (item == NULL) {
            return NULL;
        }

        GWindowDef* window = (GWindowDef*)item;
        if (window->rawHandle == (uintptr_t)hwnd) {
            return (GWindowDef*)GVector_Get(windowVector, i);
        }
    }
        
    return NULL;
}

void ButtonDown(GWindowDef* windowDef, LPARAM lParam, uint8_t button) {
    POINTS pt = MAKEPOINTS(lParam);
    POINT ptClient = { pt.x, pt.y };
    ScreenToClient(windowDef->rawHandle, &ptClient);

    GWindowPoint buttonDownLocation = {ptClient.x, ptClient.y};
    if (windowDef->buttonDownDelegate != NULL) {
        (windowDef->buttonDownDelegate)(windowDef->userData, windowDef, buttonDownLocation, button);
    }
}

void ButtonUp(GWindowDef* windowDef, LPARAM lParam, uint8_t button) {
    POINTS pt = MAKEPOINTS(lParam);
    POINT ptClient = { pt.x, pt.y };
    ScreenToClient(windowDef->rawHandle, &ptClient);

    GWindowPoint buttonUpLocation = {ptClient.x, ptClient.y};
    if (windowDef->buttonUpDelegate != NULL) {
        (windowDef->buttonUpDelegate)(windowDef->userData, windowDef, buttonUpLocation, button);
    }
}

