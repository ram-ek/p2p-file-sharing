#include "headers.h"
#include "cmdcall.h"

int accept_request(char* cmd) {
    return 0;
}

int list_groups(char* cmd) {
    return 0;
}

int list_files(char* cmd) {
    return 0;
}

int upload_file(char* cmd) {
    return 0;
}

int download_file(char* cmd) {
    return 0;
}

int show_downloads(char* cmd) {
    return 0;
}

int stop_share(char* cmd) {
    return 0;
}

int invalid(char* cmd) {
    console_write("Invalid command.\n");
    return 0;
}