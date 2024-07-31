#include "headers.h"
#include "cmdcall.h"

int create_user(struct command* cmd) {
    if(cmd->argc != cmd->argv.size()) {
        if(send(cmd->sock_out, "422 Invalid command format.", SIZE_10240, 0) < 0)
            panic("Error sending response to peer.");
    }

    string username = cmd->argv[0], password = cmd->argv[1];
    if(user_list.find(username) != user_list.end()) {
        if(send(cmd->sock_out, "409 User already exists.", SIZE_10240, 0) < 0)
            panic("Error sending response to peer.");
    }

    user_list[username] = password;
    if(send(cmd->sock_out, "200 User created successfully.", SIZE_10240, 0) < 0)
        panic("Error sending response to peer.");

    return 0;
}

int login(struct command* cmd) {
    if(send(cmd->sock_out, "200 ramek", SIZE_10240, 0) < 0)
        panic("Error sending response to peer.");

    return 0;
}

int create_group(struct command* cmd) {

}

int join_group(struct command* cmd) {

}

int leave_group(struct command* cmd) {

}

int list_request(struct command* cmd) {

}

int accept_request(struct command* cmd) {

}

int list_groups(struct command* cmd) {

}

int list_files(struct command* cmd) {

}

int upload_file(struct command* cmd) {

}

int download_file(struct command* cmd) {

}

int logout(struct command* cmd) {

}

int show_downloads(struct command* cmd) {

}

int stop_share(struct command* cmd) {

}

int invalid(struct command* cmd) {

}

int quit(struct command* cmd) {
    if(send(cmd->sock_out, "200", SIZE_10240, 0) < 0)
        panic("Error sending response to peer.");

    return 1;
}