#include "headers.h"
#include "cmd_defs.h"

static map<string, int> login_response = {
    { STATUS_OK_CODE,     0 },
    { NO_USER_CODE,       3 },
    { INVALID_CRED_CODE,  4 }
};

static int status_ok(char* payload) {
    user = payload;
    console_write(payload);
    return 0;
}

static int no_user(char* payload) {
    console_write(payload);
    return 0;
}

static int invalid_cred(char* payload) {
    console_write(payload);
    return 0;
}

static int (*response_handler[])(char* payload) = {
    [STATUS_OK]     =status_ok,
    [NO_USER]       =no_user,
    [INVALID_CRED]  =invalid_cred
};

/*
    Sends login request to tracker in format login <username> <password>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NO_USER), 401(INVALID_CRED)
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
    
    if(login_response.find(response_code) == login_response.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[login_response[response_code]](payload);
}