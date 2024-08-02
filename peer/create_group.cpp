#include "headers.h"
#include "cmd_defs.h"

static map<string, int> responses = {
    { STATUS_OK_CODE,       STATUS_OK },
    { INVALID_FORMAT_CODE,  INVALID_FORMAT },
    { UNAUTHORIZED_CODE,    UNAUTHORIZED }
};

static int status_ok(char* payload) {
    console_write(payload);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int unauthorized(char* payload) {
    console_write(payload);
    return 0;
}

static int (*response_handler[])(char* payload) = {
    [STATUS_OK]         =status_ok,
    [INVALID_FORMAT]    =invalid_format,
    [UNAUTHORIZED]      =unauthorized
};

/*
    Sends create group request to tracker in format create_group <group_id>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 401(UNAUTHORIZED)
*/
int create_group(char* cmd) {
    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");
    
    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.\n");
    
    char* response_code = strtok(response, WHITESPACE);
    if(!response_code) {
        cout << response << '\n';
        console_write("Unexpected response from tracker. No response code found.\n");
        return 0;
    }
    
    char* payload = strtok(NULL, "");
    if(!payload) {
        console_write("Unexpected response from tracker. No payload found.\n");
        return 0;
    }
    
    if(responses.find(response_code) == responses.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[responses[response_code]](payload);
}