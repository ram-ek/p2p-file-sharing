#include "headers.h"

struct command {
    int sock_out;
    int cmd_num;
    int argc;
    vector<string> argv;
    string msg;
};

#define CREATE_USER         1
#define LOGIN               2
#define CREATE_GROUP        3
#define JOIN_GROUP          4
#define LEAVE_GROUP         5
#define LIST_REQUEST        6
#define ACCEPT_REQUEST      7
#define LIST_GROUPS         8
#define LIST_FILES          9
#define UPLOAD_FILE         10
#define DOWNLOAD_FILE       11
#define LOGOUT              12
#define SHOW_DOWNLOADS      13
#define STOP_SHARE          14
#define INVALID             15
#define QUIT                16

extern int create_user(struct command* cmd);
extern int login(struct command* cmd);
extern int create_group(struct command* cmd);
extern int join_group(struct command* cmd);
extern int leave_group(struct command* cmd);
extern int list_request(struct command* cmd);
extern int accept_request(struct command* cmd);
extern int list_groups(struct command* cmd);
extern int list_files(struct command* cmd);
extern int upload_file(struct command* cmd);
extern int download_file(struct command* cmd);
extern int logout(struct command* cmd);
extern int show_downloads(struct command* cmd);
extern int stop_share(struct command* cmd);
extern int invalid(struct command* cmd);
extern int quit(struct command* cmd);