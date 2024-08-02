#include "headers.h"
#include "cmd_defs.h"

static int status_ok(char* payload) {
    user = payload;
    console_write("User logged in successfully.\n");
    return 0;
}

static int no_user_found(char* payload) {
    console_write(payload);
    return 0;
}

static int unauthorized(char* payload) {
    console_write(payload);
    return 0;
}

static map<string, int(*)(char*)> response_handler = {
    { STATUS_OK_CODE,       status_ok },
    { NOT_FOUND_CODE,       no_user_found },
    { UNAUTHORIZED_CODE,    unauthorized }
};

/*
    Sends login request to tracker in format login <username> <password>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NO_USER_FOUND), 401(UNAUTHORIZED)
*/
int login(char* cmd) {
    if(user) {
        console_write(("User " + string(user) + " already logged in.\n").c_str());
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
    
    if(response_handler.find(response_code) == response_handler.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[response_code](payload);
}