// structures
struct Client;
struct Server;
struct User;
struct Group;
struct UploadedFiles;

// command types
#define CHUNK_SIZE      524288
#define EXIT            "exit"
#define CREATE_USER     "create_user"
#define LOGIN           "login"
#define CREATE_GROUP    "create_group"
#define JOIN_GROUP      "join_group"
#define LEAVE_GROUP     "leave_group"
#define LIST_REQUESTS   "list_requests"
#define ACCEPT_REQUEST  "accept_request"
#define LIST_GROUPS     "list_groups"
#define LIST_FILES      "list_files"
#define UPLOAD_FILE     "upload_file"
#define DOWNLOAD_FILE   "download_file"
#define LOGOUT          "logout"
#define SHOW_DOWNLOADS  "show_downloads"
#define STOP_SHARE      "stop_share"