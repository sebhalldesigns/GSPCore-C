#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"

#include "GSPCore/GLog.h"
#include "internal/include/GVector.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef UNICODE
#define UNICODE
#endif 
#include <Windows.h>
#include <Windowsx.h>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GL/wglew.h"
#include <GL/gl.h>
#include <wchar.h>


const wchar_t CLASS_NAME[]  = L"GSPCore Window";
static MSG win32Msg;

// common statics
static GVector windowVector = NULL;

typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int);
typedef HGLRC(WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);

bool InitOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
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

    if (!glewInit()) {
        DEBUG_LOG(ERROR, "Failed to start GLEW");
        return false;
    }
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
    HDC hDC;
    HGLRC hRC;
    if (!InitOpenGL(handle, &hDC, &hRC)) {
         DEBUG_LOG(ERROR, "Failed to create an OpenGL context.");
    }
        
    
    window->title = info.title;
    window->rawHandle = (uintptr_t)handle;
    window->width = info.width;
    window->height = info.height;

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
            glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            SwapBuffers(GetDC(hwnd));
            break;

        case WM_SIZE:
            int newWidth = LOWORD(lParam);
            int newHeight = HIWORD(lParam);

             if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef, newSize);
                }
            }

            break;
        case WM_DISPLAYCHANGE:
            InvalidateRect(hwnd, NULL, true);
            break;
        case WM_MOUSEMOVE:

            GWindowPoint motionLocation = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            if (windowDef->pointerMoveDelegate != NULL) {
                (windowDef->pointerMoveDelegate)(windowDef, motionLocation);
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

                    if ((windowDef->willCloseDelegate)((GWindow)windowDef)) {
                        if (windowDef->closeDelegate != NULL) {
                            (windowDef->closeDelegate)((GWindow)windowDef);
                        }

                        GWindow_Close((GWindow)windowDef);
                        return 0;
                    }
                } else {
                    
                    if (windowDef->closeDelegate != NULL) {
                        (windowDef->closeDelegate)((GWindow)windowDef);
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
        (windowDef->buttonDownDelegate)(windowDef, buttonDownLocation, button);
    }
}

void ButtonUp(GWindowDef* windowDef, LPARAM lParam, uint8_t button) {
    POINTS pt = MAKEPOINTS(lParam);
    POINT ptClient = { pt.x, pt.y };
    ScreenToClient(windowDef->rawHandle, &ptClient);

    GWindowPoint buttonUpLocation = {ptClient.x, ptClient.y};
    if (windowDef->buttonUpDelegate != NULL) {
        (windowDef->buttonUpDelegate)(windowDef, buttonUpLocation, button);
    }
}

bool InitOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32, // Color depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // Depth buffer size
        8,  // Stencil buffer size
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    *hDC = GetDC(hWnd);
    if (*hDC == NULL)
        return false;

    int pixelFormat = ChoosePixelFormat(*hDC, &pfd);
    if (pixelFormat == 0)
        return false;

    if (!SetPixelFormat(*hDC, pixelFormat, &pfd))
        return false;

    *hRC = wglCreateContext(*hDC);
    if (*hRC == NULL)
        return false;

    if (!wglMakeCurrent(*hDC, *hRC))
        return false;

    // Initialize OpenGL extensions (optional)
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB == NULL)
        return false;

    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC tempContext = *hRC;
    *hRC = wglCreateContextAttribsARB(*hDC, NULL, attribs);
    if (*hRC == NULL) {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(tempContext);
        return false;
    }

    wglDeleteContext(tempContext);

    if (!wglMakeCurrent(*hDC, *hRC))
        return false;

    // Optionally, set vertical sync
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 
        (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if (wglSwapIntervalEXT != NULL)
        wglSwapIntervalEXT(1); // 1 for enabling vsync, 0 to disable

    return true;
}