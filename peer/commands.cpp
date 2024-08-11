#include "headers.h"
#include "cmdcall.h"

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