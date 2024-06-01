#include "GSPCore/Basic/GApplication.h"

#include "GSPCore/Basic/GLog.h"

#ifndef UNICODE
#define UNICODE
#endif 
#include <Windows.h>
#include <Windowsx.h>
#include <wchar.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>


static MSG win32Msg;

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

GApplication GApplication_Init(GApplicationInfo info) {

    // check if there is already an app
    if (app != NULL) {
        return NULL;
    }

    GApplicationDef* appDef = calloc(1, sizeof(GApplicationDef));

    if (appDef == NULL) {
        return NULL;
    }


    appDef->info = info;
    appDef->hInstance = GetModuleHandle(NULL);
    appDef->windows = GVector_Init();

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = WindowProcedure;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == NULL) {
        DWORD errorCode = GetLastError();

        GLog(ERROR, "Failed to register a Win32 class, error code: %lu", errorCode);

        return NULL;
    }

    app = appDef;
    return app;
}

void GApplication_Free(GApplication application) {
    free(application);
}

void GApplication_SetController(GApplication application, GApplicationController controller) {
    if (application == NULL) {
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)application;

    appDef->controller = controller;

}

int GApplication_Run(GApplication application) {

    printf("RUNNING\n");
    if (application == NULL) {
        return -1;
    }

    
    GApplicationDef* appDef = (GApplicationDef*)application;

    GApplicationControllerDef* controller = (GApplicationControllerDef*)appDef->controller;

    if (controller->launchEvent == NULL) {
        GLog(FAIL, "No app created, or app missing launch callback. Exiting application.");
        return -1;
    }



    (controller->launchEvent)();

    while (GVector_Size(appDef->windows) > 0) {
        BOOL bRet;
        while ((bRet = PeekMessage(&win32Msg, NULL, 0, 0, PM_REMOVE)) != 0) {
            TranslateMessage(&win32Msg);
            DispatchMessage(&win32Msg);
        }
        Sleep(0);
    }

    GLog(INFO, "No windows open. Exiting application.");

    return 0;
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {  

    if (app == NULL) {
        return;
    }

    GApplicationDef* appDef = (GApplicationDef*)app;
    GWindowDef* windowDef = NULL;

    size_t numWindows = GVector_Size(appDef->windows);
    for (size_t i = 0; i < numWindows; i++) {
        GWindowDef* nextWindow = (GWindow)GVector_Get(appDef->windows, i);
        if (nextWindow->rawHandle == hwnd) {
            windowDef = nextWindow;
            i = numWindows; // i.e break
        }
    }

    if (windowDef == NULL) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) { 
         case WM_SIZE:
            float newWidth = (float)LOWORD(lParam);
            float newHeight = (float)HIWORD(lParam);

            if (windowDef->info.size.width != newWidth || windowDef->info.size.height != newHeight) {

                GSize newSize = { newWidth, newHeight };

                windowDef->info.size = newSize;

                if (windowDef->controller != NULL && ((GWindowControllerDef*)windowDef->controller)->resizeEvent != NULL) {
                     (((GWindowControllerDef*)windowDef->controller)->resizeEvent)(windowDef, newSize);
                }
                
            }

            break;

        case WM_CLOSE:
            GWindow_Close(windowDef);

            break;

    }

    
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
