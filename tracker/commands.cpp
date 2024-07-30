#include "cmdcall.h"

int create_user(struct command* cmd) {

}

int login(struct command* cmd) {

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
    if(send(cmd->sock_out, QUIT_CODE, SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    return 1;
}