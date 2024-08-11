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
#define GOT_CHUNK           13
#define LOGOUT              14
#define SHOW_DOWNLOADS      15
#define STOP_SHARE          16
#define INVALID             17
#define QUIT                18

typedef struct Command {
    int sock_out;
    int cmd_num;
    int argc;
    vector<string> argv;
    string msg;
} Command;

extern int create_user(Command* cmd);
extern int login(Command* cmd);
extern int create_group(Command* cmd);
extern int join_group(Command* cmd);
extern int leave_group(Command* cmd);
extern int list_requests(Command* cmd);
extern int accept_request(Command* cmd);
extern int reject_request(Command* cmd);
extern int list_groups(Command* cmd);
extern int list_files(Command* cmd);
extern int upload_file(Command* cmd);
extern int download_file(Command* cmd);
extern int got_chunk(Command* cmd);
extern int logout(Command* cmd);
extern int show_downloads(Command* cmd);
extern int stop_share(Command* cmd);
extern int invalid(Command* cmd);
extern int quit(Command* cmd);