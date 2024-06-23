#ifndef GMARKUPMANAGER_H
#define GMARKUPMANAGER_H

#include <stddef.h>

typedef struct {
    
    void* parent;
    void* sibling;
    void* child;

    char class[128];
    bool isComplete;
} GMarkupNode;

typedef enum {
    PARSE_STATE_READY,
    PARSE_STATE_OPENING_TAG, // next character must be either a / or a non-whitespace character and is the first of the class name
    PARSE_STATE_CLASS, // all characters class name until whitespace, > or
    PARSE_STATE_IN_TAG, // default state for inside tag after class has been defined
    PARSE_STATE_END_CLOSE, // after / has been found at the end of a tag - next character must be >
    PARSE_STATE_START_CLOSE, // after a / has been found after < - next character must be alphanumeric
    PARSE_STATE_CLOSE_CLASS
} GMarkupParseState;


void GMarkupManager_Load(const char* file);


#endif // GMARKUPMANAGER_H