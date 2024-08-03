#define CREATE_USER         1
#define LOGIN               2
#define CREATE_GROUP        3
#define JOIN_GROUP          4
#define LEAVE_GROUP         5
#define LIST_REQUESTS       6
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

extern int create_user(char* cmd);
extern int login(char* cmd);
extern int create_group(char* cmd);
extern int join_group(char* cmd);
extern int leave_group(char* cmd);
extern int list_requests(char* cmd);
extern int accept_request(char* cmd);
extern int list_groups(char* cmd);
extern int list_files(char* cmd);
extern int upload_file(char* cmd);
extern int download_file(char* cmd);
extern int logout(char* cmd);
extern int show_downloads(char* cmd);
extern int stop_share(char* cmd);
extern int invalid(char* cmd);
extern int quit(char* cmd);