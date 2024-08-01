#include "headers.h"

void console_write(const char* msg) {
    pthread_mutex_lock(&console_mtx);
    cout << msg << flush;
    pthread_mutex_unlock(&console_mtx);
}


void panic(const char* msg) {
    console_write(msg);
    
    if(peer_sock != 0)
        close(peer_sock);

    exit(1);
}

void get_console() {
    char inp[SIZE_1024];

    while(true) {
        bzero(inp, SIZE_1024);
        console_write(">> ");
        cin.getline(inp, SIZE_1024);
        
        if(strlen(inp) == 0)
            continue;

        // send commands to server for processing
        if(process_cmd(inp))
            break;
    }
}