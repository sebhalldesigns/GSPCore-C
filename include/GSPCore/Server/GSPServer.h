#ifndef GSPSERVER_H
#define GSPSERVER_H

#include <stdint.h>

// TYPES

typedef enum {
    TYPE_INVALID,
    TYPE_SYSTEM,
    TYPE_GAPPLICATION
} GSPCoreType;

typedef enum {
    GAPPLICATION_OPCODE_INVALID,
    GAPPLICATION_OPCODE_INIT
} GApplicationOpcode;

typedef enum {
    GAPPLICATION_FORMAT_INVALID,
    GAPPLICATION_FORMAT_CREATEINFO,
} GApplicationOpcodeFormat;

typedef struct {
    uint64_t objectId;
    uint16_t type;
    uint16_t opcode;
    uint16_t format;
    uint16_t dataSize;
} GSPServerMessage;

typedef struct {
    uint64_t processId;
    uint32_t interfaceVersion;
    uint32_t connectionRole;
} GSPServerConnectionInfo;

typedef struct {
    uint64_t serverId;
    uint32_t interfaceVersion;
    uint32_t resultCode;
} GSPServerConnectionResponse;

typedef struct {
    uint64_t clientId;
    int functionSocket;
    int eventSocket;
} GSPServerConnection;

int GSPServer_Run();


#endif // GSPSERVER_H