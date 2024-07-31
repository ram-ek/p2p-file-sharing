#include "headers.h"
#include "cmd_defs.h"

static map<string, int> login_response = {
    { STATUS_OK_CODE,     0 },
    { NO_USER_CODE,       1 },
    { INVALID_CRED_CODE,  2 }
};

static int status_ok(char* payload) {
    user = payload;
    return 0;
}

static int no_user(char* payload) {
    console_write("User not found. Please create user before logging in.\n");
    return 0;
}

static int invalid_cred(char* payload) {
    console_write("Invalid credentials.\n");
    return 0;
}

static int (*login_response_handler[])(char* payload) = {
    [STATUS_OK]     status_ok,
    [NO_USER]       no_user,
    [INVALID_CRED]  invalid_cred
};

/*
    Sends login request to tracker in format login <username> <password>
    Expects response as STATUS_CODE RESP
    Handles error responses as 404(NO USER), 400(INVALID_CRED)
*/
int login(char* cmd) {
    if(user) {
        console_write(("User " + string(user) + " already logged in.\n").c_str());
        return 0;
    }

    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.");

    char* response_code = strtok(response, " ");
    if(!response_code) {
        console_write("Unexpected response from tracker. No response code found.\n");
        return 0;
    }

    char* payload = strtok(NULL, " ");
    if(!payload) {
        console_write("Unexpected response from tracker. No payload found.\n");
        return 0;
    }
    
    if(login_response.find(response_code) == login_response.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return login_response_handler[login_response[response_code]](payload);
}