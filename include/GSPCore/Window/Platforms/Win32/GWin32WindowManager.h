#ifndef GWIN32WINDOWMANAGER_H
#define GWIN32WINDOWMANAGER_H

#include <stdbool.h>
#include <Windows.h>

#include <GSPCore/Types/GSPCoreOpaqueTypes.h>

typedef struct {
    HINSTANCE instance;
    MSG msg;
} GWin32State;

bool GWin32WindowManager_TryInit();
GWindow* GWin32WindowManager_OpenWindow();
int GWin32WindowManager_RunLoop(GApplication* app);

wchar_t* GWin32WindowManager_GetClassName();

LRESULT CALLBACK GWin32WindowManager_WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // GWIN32WINDOWMANAGER_H