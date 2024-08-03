#include "headers.h"
#include "cmdcall.h"

int create_user(struct command* cmd) {
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
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(group_list.find(cmd->argv[0]) != group_list.end()) {
        if(send(cmd->sock_out, "409 Group with same name already exists.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = new group();
    g->group_name = cmd->argv[0];
    g->group_owner = user;
    g->group_members.insert(g->group_owner);
    group_list[g->group_name] = g;

    if(send(cmd->sock_out, "200 Group created.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int join_group(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = group_list[group_name];
    if(g->group_members.find(user) != g->group_members.end()) {
        if(send(cmd->sock_out, "400 You are already a member of this group.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(g->request_list.find(user) != g->request_list.end()) {
        if(send(cmd->sock_out, "409 You have already requested to join this group.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    g->request_list.insert(user);
    if(send(cmd->sock_out, "200 Request to join group sent.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int leave_group(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = group_list[group_name];
    if(g->group_members.find(user) == g->group_members.end()) {
        if(send(cmd->sock_out, "400 You are not a member of this group.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    g->group_members.erase(user);
    if(g->group_members.size() == 0)
        group_list.erase(group_name);

    else if(g->group_owner == user)
        g->group_owner = *g->group_members.begin();

    if(send(cmd->sock_out, "200 Group left successfully.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int list_requests(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string group_name = cmd->argv[0];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(g->request_list.size() == 0) {
        if(send(cmd->sock_out, "200 No pending requests.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string resp = "";
    for(string req: g->request_list)
        resp += req + '\n';

    if(send(cmd->sock_out, ("200 "+resp).c_str(), SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int accept_request(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string group_name = cmd->argv[0], request_user = cmd->argv[1];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(g->request_list.find(request_user) == g->request_list.end()) {
        if(send(cmd->sock_out, "404 No request from user found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    g->group_members.insert(request_user);
    g->request_list.erase(request_user);
    if(send(cmd->sock_out, "200 User added to group successfully.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int reject_request(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string group_name = cmd->argv[0], request_user = cmd->argv[1];
    if(group_list.find(group_name) == group_list.end()) {
        if(send(cmd->sock_out, "404 No group found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    struct group* g = group_list[group_name];
    if(g->group_owner != user) {
        if(send(cmd->sock_out, "403 You don't have permission to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(g->request_list.find(request_user) == g->request_list.end()) {
        if(send(cmd->sock_out, "404 No request from user found.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    g->request_list.erase(request_user);
    if(send(cmd->sock_out, "200 User request rejected.\n", SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
}

int list_groups(struct command* cmd) {
    string user = get_user_from_port(cmd->sock_out);
    if(user == NO_USER) {
        if(send(cmd->sock_out, "401 You need to be logged in to execute this command.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    if(group_list.size() == 0) {
        if(send(cmd->sock_out, "200 No groups present.\n", SIZE_1024, 0) < 0)
            panic("Error sending response to peer.\n");

        return 0;
    }

    string resp = "";
    for(auto& [group_name, group]: group_list)
        resp += group_name + '\n';

    if(send(cmd->sock_out, ("200 "+resp).c_str(), SIZE_1024, 0) < 0)
        panic("Error sending response to peer.\n");

    return 0;
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