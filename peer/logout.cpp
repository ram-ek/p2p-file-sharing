#include "headers.h"
#include "cmd_defs.h"

static map<string, int> logout_response = {
    { STATUS_OK_CODE,       STATUS_OK },
    { INVALID_FORMAT_CODE,  INVALID_FORMAT }
};

static int status_ok(char* payload) {
    user = NULL;
    console_write(payload);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int (*response_handler[])(char* payload) = {
    [STATUS_OK]         =status_ok,
    [INVALID_FORMAT]    =invalid_format
};

/*
    Sends logout request to tracker in format logout
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT)
*/
int logout(char* cmd) {
    if(!user) {
        console_write("You need to be logged in to execute this command.\n");
        return 0;
    }

    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");
    
    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.\n");

    char* response_code = strtok(response, WHITESPACE);
    if(!response_code) {
        console_write("Unexpected response from tracker. No response code found.\n");
        return 0;
    }

    char* payload = strtok(NULL, "");
    if(!payload) {
        console_write("Unexpected response from tracker. No payload found.\n");
        return 0;
    }
    
    if(logout_response.find(response_code) == logout_response.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[logout_response[response_code]](payload);
}