#include "GSPCore/GSPCore.h"
#include "GSPCore/Window/GWindowManager.h"
#include "GSPCore/Window/GWindow.h"
#include "GSPCore/Server/GSPServer.h"


#include <stdio.h>

int main() {

    if (!GSPCore_Init()) {
        return -1;
    }

    GApplication app;
    app.title = "MyApp";
    app.majorVersion = 0;
    app.minorVersion = 1;
    app.rootWindow = NULL;

    GWindow* window = GWindowManager_OpenWindow();
    app.rootWindow = window;

    return GApplicationManager_Run(&app);
}