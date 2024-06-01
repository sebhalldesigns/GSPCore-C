#ifndef GAPPLICATION_H
#define GAPPLICATION_H

// GApplication.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GApplication is the root application structure
// for working with the GSPCore system

#include "GWindow.h"
#include "GVector.h"
#include "../Controllers/GApplicationController.h"

#include <stdbool.h>

// MARK: DEFINITION
typedef void* GApplication;

typedef void* GApplicationController;

typedef struct {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;

    // options only relevant to desktop platforms, ignored on portable platforms
    bool startSilently;
    int startWidth;
    int startHeight;
} GApplicationInfo;

// MARK: CONSTRUCTOR

// constructor requires info to create. Maybe info should only be constant for that object.
GApplication GApplication_Init(GApplicationInfo info);
void GApplication_Free(GApplication application);


// MARK: METHODS

void GApplication_SetController(GApplication application, GApplicationController controller);

int GApplication_Run(GApplication application);
GWindow GApplication_GetRootWindow();

#ifdef GSPCORE_BUILD_IOS
// MARK: IOS BUILD

#import <UIKit/UIKit.h>

typedef struct {
    UIApplication* uiApplication;
    GApplicationController controller;
    GApplicationInfo info;
} GApplicationDef;

GApplication app;
#endif

#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

typedef struct {
    NSApplication* nsApplication;
    GApplicationController controller;
    GApplicationInfo info;
} GApplicationDef;

GApplication app;

#endif

#ifdef GSPCORE_BUILD_WIN32

#include <Windows.h>

const static wchar_t CLASS_NAME[]  = L"GSPCore Window";

typedef struct {
    HINSTANCE hInstance;
    GApplicationController controller;
    GApplicationInfo info;
    GVector windows;
} GApplicationDef;

GApplication app;

#endif


#endif // GAPPLICATION_H