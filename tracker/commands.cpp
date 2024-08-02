#include "headers.h"
#include "cmdcall.h"

int create_user(struct command* cmd) {
    // if(cmd->argc != cmd->argv.size()) {
    //     if(send(cmd->sock_out, "422 Invalid command format.\n", SIZE_1024, 0) < 0)
    //         panic("Error sending response to peer.\n");

    //     return 0;
    // }

    string username = cmd->argv[0], password = cmd->argv[1];
    if(user_list.find(username) != user_list.end()) {
        if(send(cmd->sock_out, "409 User already exists.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    user_list[username] = password;
    if(send(cmd->sock_out, "200 User created successfully.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");
    
    return 0;
}

int login(struct command* cmd) {
    // if(cmd->argc != cmd->argv.size()) {
    //     if(send(cmd->sock_out, "422 Invalid command format.\n", SIZE_1024, 0) < 0)
    //         panic("Error sending response to peer.\n");

    //     return 0;
    // }

    string username = cmd->argv[0], password = cmd->argv[1];
    if(user_list.find(username) == user_list.end()) {
        if(send(cmd->sock_out, "404 No user exists.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(password != user_list[username]) {
        if(send(cmd->sock_out, "401 Invalid credentials.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(send(cmd->sock_out, ("200 "+username).c_str(), SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    bind_user_to_port(cmd->sock_out, username);

    return 0;
}

int create_group(struct command* cmd) {
    if(port_to_user[cmd->sock_out] == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = new group();
    g->group_name = cmd->argv[0];
    g->group_owner = port_to_user[cmd->sock_out];
    g->group_members.insert(g->group_owner);
    group_list[g->group_name] = g;

    if(send(cmd->sock_out, "200 Group created.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
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
    if(cmd->argc != cmd->argv.size()) {
        if(send(cmd->sock_out, "422 Invalid command format.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(send(cmd->sock_out, "200 User logged out successfully.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    bind_user_to_port(cmd->sock_out, NO_USER);

    return 0;
}

int show_downloads(struct command* cmd) {

}

int stop_share(struct command* cmd) {

}

int invalid(struct command* cmd) {
    if(send(cmd->sock_out, "422 Invalid command format.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    return 0;
}

int quit(struct command* cmd) {
    if(send(cmd->sock_out, "200", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.");

    // free up the port mapping for other users
    remove_port_bind(cmd->sock_out);

    return 1;
}