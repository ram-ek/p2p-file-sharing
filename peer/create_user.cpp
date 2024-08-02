#include "headers.h"
#include "cmd_defs.h"

static map<string, int> create_user_response = {
    { STATUS_OK_CODE,       0 },
    { INVALID_FORMAT_CODE,  1 },
    { USER_EXIST_CODE,      2 }
};

static int status_ok(char* payload) {
    console_write(payload);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int user_exist(char* payload) {
    console_write(payload);
    return 0;
}

static int (*response_handler[])(char* payload) = {
    [STATUS_OK]         =status_ok,
    [INVALID_FORMAT]    =invalid_format,
    [USER_EXIST]        =user_exist
};

/*
    Sends create user request to tracker in format create_user <username> <password>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NO USER), 409(USER_EXIST)
*/
int create_user(char* cmd) {
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

    if(create_user_response.find(response_code) == create_user_response.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[create_user_response[response_code]](payload);
}