#include "GSPCore/GSPCore.h"

#include <stdio.h>




void MouseEnter(GView view) {
    printf("MOUSE ENTER\n");

    GViewDef* viewDef = (GViewDef*)view;
    viewDef->backgroundColor = (GColor) { 0.0f, 0.0f, 1.0f, 1.0f };
    GView_Render(view);

    //GWindow_Render(window);
}

void MouseExit(GView view) {
    printf("MOUSE EXIT\n");

    
    GViewDef* viewDef = (GViewDef*)view;
    viewDef->backgroundColor = (GColor) { 1.0f, 0.0f, 1.0f, 1.0f };
    GView_Render(view);

        //GWindow_Render(window);

}
void WindowResized(GWindow window, GSize size) {
    printf("WINDOW RESIZED: %f %f\n", size.width, size.height);
}

void AppLaunched(GApplication application) {

    printf("App launched!\n");
    
    GWindowInfo info;
    info.title = "MyApp Window";
    info.size.width = 800;
    info.size.height = 600; 

    GWindow window = GWindow_Init(info);

    
    GWindowController windowController = GWindowController_Init();
    GWindowController_SetResizeEvent(windowController, WindowResized);

    GWindow_SetController(window, windowController);

    GApplication_SetMainWindow(application, window);


    GViewInfo viewInfo;
    viewInfo.backgroundColor = (GColor) { 1.0f, 1.0f, 0.0f, 1.0f };
    GView rootView = GView_Init(viewInfo);
    GWindow_SetRootView(window, rootView);

    GViewDef* rootViewDef = (GViewDef*)rootView;
    rootViewDef->layout = LAYOUT_DOCK;

    

    GViewInfo view2Info;
    view2Info.frame = (GRect) { 100.0, 100.0, 100.0, 100.0 };
    view2Info.backgroundColor = (GColor) { 1.0f, 0.0f, 1.0f, 1.0f };
    GView view2 = GView_Init(view2Info);
    GView_AddSubview(rootView, view2);

    GViewDef* view2Def = (GViewDef*)view2;
    view2Def->dockLocation = DOCK_BOTTOM;


    GViewController view2Controller = GViewController_Init();
    GViewController_SetMouseEnterEvent(view2Controller, MouseEnter);
    GViewController_SetMouseExitEvent(view2Controller, MouseExit);
    GView_SetController(view2, view2Controller);

    GView_Render(view2);


    GViewInfo view3Info;
    view3Info.frame = (GRect) { 100.0, 100.0, 100.0, 100.0 };
    view3Info.backgroundColor = (GColor) { 1.0f, 0.0f, 0.0f, 1.0f };
    GView view3 = GView_Init(view3Info);
    GView_AddSubview(rootView, view3);
    GView_Render(view3);

    GDrawing drawing = GDrawing_Init();
    GView_SetDrawing(view3, drawing);

}

int main(int argc, char* argv[]) {


    printf("HELLO WORLD\n");    

    GApplicationInfo info;
    info.title = "MyApp";
    info.developer = "Seb Hall";
    info.majorVersion = 0;
    info.minorVersion = 1;

    GApplication app = GApplication_Init(info);

    GApplicationController appController = GApplicationController_Init();
    GApplicationController_SetLaunchEvent(appController, AppLaunched);

    GApplication_SetController(app, appController);

    return GApplication_Run(app);
}
