#include <GSPCore/GSPCore.h>

#include <GSPCore/Server/GSPServer.h>

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <time.h>

int GSPServer_Run() {
    

    GLog(INFO, "Starting GSPServer");

    const char* socketPath = "/tmp/GSPCore.sock";

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Server socket fd = %d\n", sock);

    // Make sure socket's file descriptor is legit.
    if (sock == -1) {
        GLog(FAIL, "Failed to create a socket");
        return -1;
    }

    // Delete any file that already exists at the address. Make sure the deletion
    // succeeds. If the error is just that the file/directory doesn't exist, it's fine.
    if (remove(socketPath) == -1 && errno != ENOENT) {
        GLog(FAIL, "Failed to remove existing socket");
        return -1;
    }

    struct sockaddr_un addr;
    
    // Make sure the address we're planning to use isn't too long.
    if (strlen(socketPath) > sizeof(addr.sun_path) - 1) {
        GLog(FAIL, "Socket path was too long");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath, sizeof(addr.sun_path) - 1);


    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        GLog(FAIL, "Failed to bind to socket");
        //return -1;
    }

    if (listen(sock, 5) == -1) {
        GLog(FAIL, "Failed to listen on socket");
        return -1;
    }

    while (true) {

        GLog(INFO, "Listening for clients...");

        int client = accept(sock, NULL, NULL);
        GLog(INFO, "Accepted client on socket %d", client);

        clock_t cstart = clock();
        clock_t cend = 0;



        GSPServerMessage message_received;
        if (recv(client, &message_received, sizeof(GSPServerMessage), 0) < 0) {
            GLog(WARNING, "Error recieving message from client %d", client);
        } else {
            GLog(INFO, "Recieved message from client %d\n %lu %u %u %u %u ", client, message_received.objectId, message_received.type, message_received.opcode, message_received.format, message_received.dataSize);
        }

        GSPServerMessage message_to_send = { 1234, 1, 1, 1, 256 }; // Example message to send
        if (send(client, &message_to_send, sizeof(GSPServerMessage), 0) < 0) {
            GLog(WARNING, "Error sending message to client %d", client);
        }

        cend = clock();
        printf ("%.3f cpu sec\n", ((double)cend - (double)cstart));


        if (close(client) == -1) {
            GLog(WARNING, "Failed to close client socket %d", client);
        } else {
            GLog(INFO, "Terminated client on socket %d", client);
        }

    }

    int client_sock = accept(sock, NULL, NULL);

    return -1;
}