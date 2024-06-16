#include <GSPCore/GSPCore.h>
#include <GSPCore/Window/Platforms/Win32/GWin32WindowManager.h>
#include <GSPCore/Render/GRenderManager.h>

#include <Windows.h>

static GWin32State state;
static GWindow* rootWindow = NULL;

const static wchar_t CLASS_NAME[]  = L"GSPCore Window";

bool GWin32WindowManager_TryInit() {

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = GWin32WindowManager_WindowProcedure;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;

    if (RegisterClass(&wc) == NULL) {
        DWORD errorCode = GetLastError();

        GLog(ERROR, "Failed to register a Win32 class, error code: %lu", errorCode);

        return false;
    }

    GLog(INFO, "Started GWin32WindowManager ok");

    return true;
}

GWindow* GWin32WindowManager_OpenWindow() {
    
    GWindow* window = calloc(1, sizeof(GWindow));
    window->platformHandles.handle = CreateWindow(
        CLASS_NAME,
        L"Sample Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if (window->platformHandles.handle == NULL) {
        DEBUG_LOG(FAIL, "Failed to create a Win32 window");
        free(window);
        return NULL;
    }

    ShowWindow(window->platformHandles.handle, SW_SHOW);

    rootWindow = window;
    return window;
}

int GWin32WindowManager_RunLoop(GApplication* app) {
    printf("RUNNING LOOP\n");

    while (true) {
        while (GetMessage(&state.msg, NULL, 0, 0)) {
            TranslateMessage(&state.msg);
            DispatchMessage(&state.msg);
        }

        printf("RUN LOOOP\n");
        Sleep(0);
    }
}

wchar_t* GWin32WindowManager_GetClassName() {
    return CLASS_NAME;
}

LRESULT CALLBACK GWin32WindowManager_WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    
    switch (msg) { 
         case WM_SIZE:

            float newWidth = (float)LOWORD(lParam);
            float newHeight = (float)HIWORD(lParam);

            if (rootWindow != NULL) {
                rootWindow->frame.size.width = newWidth;
                rootWindow->frame.size.height = newHeight;
                GRenderManager_RenderWindow(rootWindow);
                
            }


            printf("SIZE\n");

            break;

        case WM_CLOSE:
           
            printf("CLOSE\n");

            break;

    }

        return DefWindowProc(hwnd, msg, wParam, lParam);

}