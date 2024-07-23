#include <gsp_debug/gsp_debug.h>

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

void gsp_debug_log(gsp_debug_level_t level, gstring_t message, ...) {
    time_t currentTime = time(0);

    struct tm* timeInfo = localtime(&currentTime);
    
    printf("[%02d:%02d:%02d] ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);

    switch (level) {
        case INFO: {
            printf("\033[0;94mINFO\033[0m ");
            break;
        }
        case WARNING: {
            printf("\033[0;93mWARN\033[0m ");
            break;
        }
        case FAIL: {
            printf("\033[0;91mERROR\033[0m ");
            break;
        }
    }

    // print formatted message
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("\n");
}