#ifndef GVIEWCONTROLLER_H
#define GVIEWCONTROLLER_H

// GViewController.h - GSPCore 
// Created by Seb Hall in June 2024
//
// GViewController is an object that
// recieves and handles events for instances
// of GView.

typedef void* GView;

typedef void* GViewController;

typedef void(*GViewLoadedEvent)(GView view);



GViewController GViewController_Init();

void GViewController_SetLoadedEvent(GViewController viewController, GViewLoadedEvent loadedEvent);

void GViewController_Free(GViewController viewController);



#endif // GVIEWCONTROLLER_H