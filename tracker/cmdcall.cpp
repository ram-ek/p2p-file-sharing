#include "headers.h"
#include "cmdcall.h"

map<string, int> cmdcallmap = {
    {"create_user",     CREATE_USER},
    {"login",           LOGIN},
    {"create_group",    CREATE_GROUP},
    {"join_group",      JOIN_GROUP},
    {"leave_group",     LEAVE_GROUP},
    {"list_requests",   LIST_REQUESTS},
    {"accept_request",  ACCEPT_REQUEST},
    {"list_groups",     LIST_GROUPS},
    {"list_files",      LIST_FILES},
    {"upload_file",     UPLOAD_FILE},
    {"download_file",   DOWNLOAD_FILE},
    {"logout",          LOGOUT},
    {"show_downloads",  SHOW_DOWNLOADS},
    {"stop_share",      STOP_SHARE},
    {"invalid",         INVALID},
    {"quit",            QUIT}
};

static int (*cmdcalls[])(struct command* cmd) = {
    [CREATE_USER]       =create_user,
    [LOGIN]             =login,
    [CREATE_GROUP]      =create_group,
    [JOIN_GROUP]        =join_group,
    [LEAVE_GROUP]       =leave_group,
    [LIST_REQUESTS]     =list_requests,
    [ACCEPT_REQUEST]    =accept_request,
    [LIST_GROUPS]       =list_groups,
    [LIST_FILES]        =list_files,
    [UPLOAD_FILE]       =upload_file,
    [DOWNLOAD_FILE]     =download_file,
    [LOGOUT]            =logout,
    [SHOW_DOWNLOADS]    =show_downloads,
    [STOP_SHARE]        =stop_share,
    [INVALID]           =invalid,
    [QUIT]              =quit
};

static int cmdcall_args[] = {
    [CREATE_USER]       =2,
    [LOGIN]             =2,
    [CREATE_GROUP]      =1,
    [JOIN_GROUP]        =1,
    [LEAVE_GROUP]       =1,
    [LIST_REQUESTS]     =1,
    [ACCEPT_REQUEST]    =2,
    [LIST_GROUPS]       =1,
    [LIST_FILES]        =1,
    [UPLOAD_FILE]       =2,
    [DOWNLOAD_FILE]     =3,
    [LOGOUT]            =0,
    [SHOW_DOWNLOADS]    =0,
    [STOP_SHARE]        =2,
    [INVALID]           =0,
    [QUIT]              =0
};

int cmdcall(struct command* cmd) {
    if(cmd->cmd_num != INVALID && cmd->argc != cmd->argv.size()) {
        cmd->cmd_num = INVALID;
        cmd->msg = "Argument number mismatch, expected " + to_string(cmd->argc) + " got " + to_string(cmd->argv.size());
    }
    
    return cmdcalls[cmd->cmd_num](cmd);
}

int process_cmd(char* cmd_line, int* sock_out) {
    struct command cmd;
    vector<string> cmd_tokens = get_tokens(cmd_line, WHITESPACE);

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

    return cmdcall(&cmd);
}