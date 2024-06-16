#include <GSPCore/GSPCore.h>
#include <GSPCore/Application/GApplicationManager.h>
#include <GSPCore/Window/GWindowManager.h>
#include <GSPCore/Render/GRenderManager.h>
#include <GSPCore/View/GViewManager.h>

const int majorVersion = 0;
const int minorVersion = 0;

bool GSPCore_Init() {

    GLog(INFO, "Starting GSPCore v%d.%d", majorVersion, minorVersion);

    GApplicationManager_Init();
    GWindowManager_Init();
    GRenderManager_Init();
    GViewManager_Init();

    return true;

}