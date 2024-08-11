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
    {"reject_request",  REJECT_REQUEST},
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

static int (*cmdcalls[])(char* cmd) = {
    [CREATE_USER]       =create_user,
    [LOGIN]             =login,
    [CREATE_GROUP]      =create_group,
    [JOIN_GROUP]        =join_group,
    [LEAVE_GROUP]       =leave_group,
    [LIST_REQUESTS]     =list_requests,
    [ACCEPT_REQUEST]    =accept_request,
    [REJECT_REQUEST]    =reject_request,
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

int process_cmd(char* cmd) {
    // get command type
    char cmd_type[100]; // greater than max command length
    peek(cmd, cmd_type, WHITESPACE);
    
    if(strcmp(cmd_type, "") == 0)
        return 0;

    // call for invalid command
    if(cmdcallmap.find(cmd_type) == cmdcallmap.end())
        return cmdcalls[INVALID](cmd);

    // // make the call to command handler
    int cmd_num = cmdcallmap[cmd_type];
    return cmdcalls[cmd_num](cmd);
}