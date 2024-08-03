#include "headers.h"
#include "cmd_defs.h"

int quit(char* cmd) {
    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.");
    
    if(strcmp(response, STATUS_OK_CODE) != 0)
        panic("Unexpected response from tracker.");

    return 1;
}