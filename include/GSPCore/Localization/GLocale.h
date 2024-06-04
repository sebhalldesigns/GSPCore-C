#ifndef GLOCALE_H
#define GLOCALE_H

#include <string.h>
 
typedef enum {
    WRITING_DIRECTION_UNKNOWN,
    WRITING_DIRECTION_LEFT_TO_RIGHT,
    WRITING_DIRECTION_RIGHT_TO_LEFT
} GLocaleWritingDirection;

typedef struct {
    const char* tag;
    const char* localDescription;
    const char* internationalDescription;
} GLocaleIdentifier;

#endif // GLOCALE_H