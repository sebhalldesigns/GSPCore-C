#include <stdint.h>


// UPPER HALF OF dataType byte (0xF0 mask), lower half is max array index (1-16)
#define VOID 0
#define OBJECT 1
#define BOOL 2
#define BYTE 3
#define INT16 4
#define INT32 5 
#define INT64 6
#define UINT16 7
#define UINT32 8
#define UINT64 9
#define FLOAT32 10
#define FLOAT64 11
#define STRING 12
#define STRUCT 13
#define ENUM 14
#define ARBITRARY 15

// UPPER HALF OF commandByte (0xF0 mask), lower half is number of messages remaining for this call (up to 128 bytes per call)
#define INIT 0
#define GET 1
#define SET 2
#define DESTROY 15


typedef struct {
    uintptr_t destObject; // 8
    uintptr_t selector; // 8
    uint8_t comandByte; // 1
    uint8_t dataType; // 1
    uint8_t data[8];  // 12
} Packet;




