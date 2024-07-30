#include "headers.h"
#include "cmdcall.h"

struct command cmd;

map<string, int> cmdcallmap = {
    {"create_user",     1},
    {"login",           2},
    {"create_group",    3},
    {"join_group",      4},
    {"leave_group",     5},
    {"list_request",    6},
    {"accept_request",  7},
    {"list_groups",     8},
    {"list_files",      9},
    {"upload_file",     10},
    {"download_file",   11},
    {"logout",          12},
    {"show_downloads",  13},
    {"stop_share",      14},
    {"invalid",         15},
    {"quit",            16},
};

static int (*cmdcalls[])(struct command* cmd) = {
    [CREATE_USER]       create_user,
    [LOGIN]             login,
    [CREATE_GROUP]      create_group,
    [JOIN_GROUP]        join_group,
    [LEAVE_GROUP]       leave_group,
    [LIST_REQUEST]      list_request,
    [ACCEPT_REQUEST]    accept_request,
    [LIST_GROUPS]       list_groups,
    [LIST_FILES]        list_files,
    [UPLOAD_FILE]       upload_file,
    [DOWNLOAD_FILE]     download_file,
    [LOGOUT]            logout,
    [SHOW_DOWNLOADS]    show_downloads,
    [STOP_SHARE]        stop_share,
    [INVALID]           invalid,
    [QUIT]              quit,
};

static int cmdcall_args[] = {
    [CREATE_USER]       2,
    [LOGIN]             2,
    [CREATE_GROUP]      1,
    [JOIN_GROUP]        1,
    [LEAVE_GROUP]       1,
    [LIST_REQUEST]      1,
    [ACCEPT_REQUEST]    2,
    [LIST_GROUPS]       1,
    [LIST_FILES]        1,
    [UPLOAD_FILE]       2,
    [DOWNLOAD_FILE]     3,
    [LOGOUT]            0,
    [SHOW_DOWNLOADS]    0,
    [STOP_SHARE]        2,
    [INVALID]           0,
    [QUIT]              0,
};

int cmdcall() {
    if(cmd.cmd_num != INVALID && cmd.argc != cmd.argv.size()) {
        cmd.cmd_num = INVALID;
        cmd.msg = "Argument number mismatch, expected " + to_string(cmd.argc) + " got " + to_string(cmd.argv.size());
    }

    return cmdcalls[cmd.cmd_num](&cmd);
}

int processcmd(char* inp, int* sock_out) {
    vector<string> cmd_tokens = get_tokens(inp, " ");

    cmd.sock_out = *sock_out;
    if(cmdcallmap.find(cmd_tokens[0]) == cmdcallmap.end()) {
        cmd.cmd_num = INVALID;
        cmd.argc = cmdcall_args[INVALID];
        cmd.argv = {};
        cmd.msg = "Invalid command.";
    }
    else {
        cmd.cmd_num = cmdcallmap[cmd_tokens[0]];
        cmd.argc = cmdcall_args[cmd.cmd_num];
        for(int i=1; i<cmd_tokens.size(); i++)
            cmd.argv.push_back(cmd_tokens[i]);
    }

    return cmdcall();
}