#ifndef GSPCORE_H
#define GSPCORE_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#include "Types/GSPCoreOpaqueTypes.h"
#include "Types/GSPCoreGraphics.h"
#include "Types/GSPCoreGeometry.h"


#include "Application/GApplication.h"
#include "Application/GApplicationManager.h"

#include "Window/GWindow.h"
#include "Window/GWindowManager.h"
#include "Controllers/GApplicationController.h"

#include "Resource/GResourceManager.h"

#include "Debug/GLog.h"

// top-level init function to be called first
bool GSPCore_Init();

#endif // GSPCORE_H