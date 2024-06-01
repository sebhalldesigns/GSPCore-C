#ifndef GWINDOWCONTROLLER_H
#define GWINDOWCONTROLLER_H

// GWindowController.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GWindowController is an object that
// recieves and handles events for instances
// of GWindow.

typedef void* GWindow;

typedef void* GWindowController;

typedef void(*GWindowCloseEvent)(GWindow window);
typedef bool(*GWindowWillCloseEvent)(GWindow window);
typedef void (*GWindowResizeEvent)(GWindow window, GSize newSize);


GWindowController GWindowController_Init();

void GWindowController_Free(GWindowController windowController);



#endif // GWINDOWCONTROLLER_H