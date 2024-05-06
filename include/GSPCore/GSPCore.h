#ifndef GSPCORE_H
#define GSPCORE_H

#include "GApplication.h"

// Welcome to the GSPCore library, a part of the OpenGSP project!
// This file contains the top-level interface of the lirbary,
// designed as a C-style interface for compatibility.


// Main GSPCore function. Takes an instance of GApplication. 
// Returns 0 when application is complete, or an error code to describe the reason for quitting.

int GSPRun(GApplication app);



#endif // GSPCORE_H