#ifndef GVIEWCONTROLLER_H
#define GVIEWCONTROLLER_H

// GViewController.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GViewController is an object that
// recieves and handles events for instances
// of GView.

#include "../Types/GSPCoreOpaqueTypes.h"


typedef void(*GViewLoadedEvent)(GView view);

typedef void(*GViewMouseEnterEvent)(GView view);
typedef void(*GViewMouseExitEvent)(GView view);



GViewController GViewController_Init();

void GViewController_Free(GViewController viewController);

void GViewController_SetLoadedEvent(GViewController viewController, GViewLoadedEvent loadedEvent);
void GViewController_SetMouseEnterEvent(GViewController viewController, GViewMouseEnterEvent mouseEnterEvent);
void GViewController_SetMouseExitEvent(GViewController viewController, GViewMouseExitEvent mouseExitEvent);


#ifdef GSPCORE_BUILD

#include <stdlib.h>

typedef struct {
    GViewLoadedEvent loadedEvent;
    GViewMouseEnterEvent mouseEnterEvent;
    GViewMouseExitEvent mouseExitEvent;
} GViewControllerDef;



#endif

#endif // GVIEWCONTROLLER_H