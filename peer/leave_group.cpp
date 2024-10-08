#include "headers.h"
#include "cmd_defs.h"

static int status_ok(char* payload) {
    console_write(payload);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int no_group_found(char* payload) {
    console_write(payload);
    return 0;
}

static int bad_request(char* payload) {
    console_write(payload);
    return 0;
}

static int unauthorized(char* payload) {
    console_write(payload);
    return 0;
}

static map<string, int(*)(char*)> response_handler = {
    { STATUS_OK_CODE,       status_ok },
    { INVALID_FORMAT_CODE,  invalid_format },
    { NOT_FOUND_CODE,       no_group_found },
    { BAD_REQUEST_CODE,     bad_request },
    { UNAUTHORIZED_CODE,    unauthorized }
};

/*
    Sends leave group request to tracker in format leave_group <group_id>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NO_GROUP_FOUND), 400(BAD_REQUEST), 401(UNAUTHORIZED)
*/
int leave_group(char* cmd) {
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

    if(response_handler.find(response_code) == response_handler.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[response_code](payload);
}