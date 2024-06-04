#ifndef GLOCALSTRINGPROVIDER_H
#define GLOCALSTRINGPROVIDER_H

#include <string.h>
#include "GLocale.h"

void GLocalStringProvider_SetLocale(GLocaleIdentifier identifer);

const char* GLocalStringProvider_ResolveString(const char* key);

#endif // GLOCALSTRINGPROVIDER_H