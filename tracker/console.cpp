#include "headers.h"

void console_write(const char* msg) {
    pthread_mutex_lock(&console_mtx);
    cout << msg << flush;
    pthread_mutex_unlock(&console_mtx);
}

void panic(const char* msg) {
    console_write(msg);
    
    if(tracker_sock != 0)
        close(tracker_sock);

    exit(1);
}

void get_console() {
    char inp[SIZE_10240];

    while(true) {
        console_write(">> ");
        cin.getline(inp, SIZE_10240);

        vector<string> inp_tok = get_tokens(inp, " ");
        if(inp_tok.size() == 0)
            continue;

        if(inp_tok.size() == 1 && inp_tok[0] == "quit")
            break;
    }
}