#include "headers.h"
#include "cmdcall.h"

int join_group(char* cmd) {
    return 0;
}

int leave_group(char* cmd) {
    return 0;
}

int list_request(char* cmd) {
    return 0;
}

int accept_request(char* cmd) {
    return 0;
}

int list_groups(char* cmd) {
    return 0;
}

int list_files(char* cmd) {
    return 0;
}

int upload_file(char* cmd) {
    return 0;
}

int download_file(char* cmd) {
    return 0;
}

int show_downloads(char* cmd) {
    return 0;
}

int stop_share(char* cmd) {
    return 0;
}

int invalid(char* cmd) {
    console_write("Invalid command.\n");
    return 0;
}

int quit(char* cmd) {
    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.");
    
    if(strcmp(response, "200") != 0)
        panic("Unexpected response from tracker.");

    return 1;
}