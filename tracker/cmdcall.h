#include "headers.h"

#define CREATE_USER         1
#define LOGIN               2
#define CREATE_GROUP        3
#define JOIN_GROUP          4
#define LEAVE_GROUP         5
#define LIST_REQUESTS       6
#define ACCEPT_REQUEST      7
#define REJECT_REQUEST      8
#define LIST_GROUPS         9
#define LIST_FILES          10
#define UPLOAD_FILE         11
#define DOWNLOAD_FILE       12
#define LOGOUT              13
#define SHOW_DOWNLOADS      14
#define STOP_SHARE          15
#define INVALID             16
#define QUIT                17

struct command {
    int sock_out;
    int cmd_num;
    int argc;
    vector<string> argv;
    string msg;
};

extern int create_user(struct command* cmd);
extern int login(struct command* cmd);
extern int create_group(struct command* cmd);
extern int join_group(struct command* cmd);
extern int leave_group(struct command* cmd);
extern int list_requests(struct command* cmd);
extern int accept_request(struct command* cmd);
extern int reject_request(struct command* cmd);
extern int list_groups(struct command* cmd);
extern int list_files(struct command* cmd);
extern int upload_file(struct command* cmd);
extern int download_file(struct command* cmd);
extern int logout(struct command* cmd);
extern int show_downloads(struct command* cmd);
extern int stop_share(struct command* cmd);
extern int invalid(struct command* cmd);
extern int quit(struct command* cmd);