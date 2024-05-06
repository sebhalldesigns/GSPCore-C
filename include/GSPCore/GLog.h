#ifndef GLOG_H
#define GLOG_H


#ifdef DEBUG 
#define DEBUG_LOG(level, ...) GLog(level, __VA_ARGS__)
#else
#define DEBUG_LOG(level, ...) ((void)0)
#endif


typedef enum {
    INFO,
    WARNING,
    ERROR,
    FAILURE
} GLogLevel;

void GLog(GLogLevel level, const char* message, ...);

#endif // GLOG_H