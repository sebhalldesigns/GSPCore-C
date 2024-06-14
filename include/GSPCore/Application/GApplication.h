#ifndef GAPPLICATION_H
#define GAPPLICATION_H

// GApplication.h - GSPCore 
// Created by Seb Hall in March 2024
//
// GApplication is the root application structure
// for working with the GSPCore system


#include "../Types/GSPCoreOpaqueTypes.h"

#include "../Controllers/GApplicationController.h"
#include <stdbool.h>

typedef enum {
    ENVIRONMENT_UNIX,
    ENVIRONMENT_MACOS,
    ENVIRONMENT_WIN32,
    ENVIRONMENT_IOS,
    ENVIRONMENT_ANDROID,
    ENVIRONMENT_WASM
} GApplicationEnvironment;

typedef struct {
    bool isSet;
    void* data;
} GOptional;

typedef struct {
    const char* title;
    const char* developer;
    int majorVersion;
    int minorVersion;

    // options only relevant to desktop platforms, ignored on portable platforms
    bool startSilently;
    int startWidth;
    int startHeight;

    GApplicationController applicationController;
} GApplication;

// MARK: METHODS

GWindow GApplication_GetMainWindow();
void GApplication_SetMainWindow(GWindow window);
GApplicationEnvironment GApplication_GetEnvironment();


int GApplication_Run(GApplication *app);


#ifdef GSPCORE_BUILD_WASM

#include <emscripten.h>

typedef struct {
    GApplicationController controller;
    GApplicationInfo info;
    GWindow mainWindow;
} GApplicationDef;

static GApplication app;

EMSCRIPTEN_KEEPALIVE
void WasmMainLoop();

EMSCRIPTEN_KEEPALIVE
void WasmMouseMove(double mouseX, double mouseY);

EMSCRIPTEN_KEEPALIVE
void WasmWindowResize(double width, double height);

EMSCRIPTEN_KEEPALIVE
void WasmLightMode();

EMSCRIPTEN_KEEPALIVE
void WasmDarkMode();

#endif

#ifdef GSPCORE_BUILD_IOS
// MARK: IOS BUILD

#import <UIKit/UIKit.h>

typedef struct {
    UIApplication* uiApplication;
    GApplicationController controller;
    GApplicationInfo info;
    GWindow mainWindow;
} GApplicationDef;

static GApplication app;
#endif

#ifdef GSPCORE_BUILD_MACOS

// MARK: MACOS BUILD

#import <Cocoa/Cocoa.h>

typedef struct {
    NSApplication* nsApplication;
    GApplicationController controller;
    GApplicationInfo info;
    GWindow mainWindow;
    GVector windows;
} GApplicationDef;

static GApplication app;

#endif

#ifdef GSPCORE_BUILD_WIN32

#include <Windows.h>

const static wchar_t CLASS_NAME[]  = L"GSPCore Window";

typedef struct {
    HINSTANCE hInstance;
    GApplicationController controller;
    GApplicationInfo info;
    GWindow mainWindow;
    GVector windows;
} GApplicationDef;

static GApplication app;

#endif




#endif // GAPPLICATION_H