#include "headers.h"

void console_write(const char* msg) {
    pthread_mutex_lock(&console_mtx);
    cout << msg << flush;
    pthread_mutex_unlock(&console_mtx);
}


void panic(const char* msg) {
    console_write(msg + '\n');
    
    if(peer_sock != 0)
        close(peer_sock);

    exit(1);
}

bool send_cmd(char* cmd) {
    // do sanity checks
    

    // tokenize command
    vector<string> inp_tok = get_tokens(cmd, " ");
    if(inp_tok.size() == 0)
        return true;

    // send command to tracker
    if(send(peer_sock, cmd, strlen(cmd), 0) < 0)
        panic("Error sending command.");

    char response[SIZE_1024];   // repition
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.");

    // quit out of client
    if(strcmp(response, QUIT_CODE) == 0)
        return false;
    
    return true;
}

void get_console() {
    char inp[SIZE_1024];

    while(true) {
        console_write(">> ");
        cin.getline(inp, SIZE_1024);

        // send commands to server for processing
        if(!send_cmd(inp))
            break;
    }

    close(peer_sock);
}