#ifndef GMARKUPMANAGER_H
#define GMARKUPMANAGER_H

#include <stddef.h>

typedef struct {
    
    void* sibling;
    void* firstChild;
    size_t numChildren;

    const char* class;
    bool isComplete;
} GMarkupNode;

typedef enum {
    PARSE_STATE_READY,
    PARSE_STATE_OPENING_TAG, // next character must be a non-whitespace character
    PARSE_STATE_CLASS,
    PARSE_STATE_IN_TAG,
    PARSE_STATE_CLOSING_TAG
} GMarkupParseState;


void GMarkupManager_Load(const char* file);


#endif // GMARKUPMANAGER_H