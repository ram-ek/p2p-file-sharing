#include "headers.h"
#include "cmdcall.h"

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

static int (*cmdcalls[])(char* cmd) = {
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

int processcmd(char* cmd) {
    // get command type
    const char* cmd_type = peek(cmd, " ").c_str();
    
    if(!cmd_type)
        return 0;

    // call for invalid command
    if(cmdcallmap.find(cmd_type) == cmdcallmap.end())
        return cmdcalls[INVALID](cmd);

    // // make the call to command handler
    int cmd_num = cmdcallmap[cmd_type];
    return cmdcalls[cmd_num](cmd);
}