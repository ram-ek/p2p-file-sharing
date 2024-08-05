#include "headers.h"

void* handle_client(void* arg) {
    int* sock_out = (int*) arg;
    // console_write(("Connected to socket " + to_string(*sock_out) + '\n').c_str());
    // console_write(">> ");
    char cmd[SIZE_1024];
    
    while(true) {
        bzero(cmd, SIZE_1024);
        
        if(recv(*sock_out, cmd, SIZE_1024, 0) < 0)
            panic("Error receiving command from peer.\n");
        
        if(process_cmd(cmd, sock_out))
            break;
    }

    // console_write(("Disconnected to socket " + to_string(*sock_out) + '\n').c_str());
    // console_write(">> ");
    
    close(*sock_out);
    return NULL;
}

void* run_server(void* arg) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if((tracker_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        panic("Error making socket for server.\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tracker_port);

    if(inet_pton(AF_INET, tracker_ip, &addr.sin_addr) <= 0)
        panic("Error with tracker ip address.\n");

    if(bind(tracker_sock, (const struct sockaddr*)&addr, addrlen) < 0)
        panic("Error binding with socket.\n");

    if(listen(tracker_sock, BACKLOG) < 0)
        panic("Error listening to socket.\n");

    while(true) {
        int* sock_out = (int*)malloc(sizeof(int));

        if((*sock_out = accept(tracker_sock, (struct sockaddr*)&addr, &addrlen)) < 0)
            panic("Error accepting client connection.\n");
        
        // getting server peerid from peer
        char peerid[SIZE_1024];
        bzero(peerid, SIZE_1024);
        if(recv(*sock_out, peerid, SIZE_1024, 0) < 0)
            panic("Error receiving command from peer.\n");

        bind_peerid_to_sock(*sock_out, peerid);
        bind_user_to_sock(*sock_out, NO_USER);
        
        // dispatch thread to handle client
        pthread_t client_thread;
        if(pthread_create(&client_thread, NULL, handle_client, sock_out) != 0) {
            console_write("Failed to start client thread.\n");
            continue;
        }

        pthread_detach(client_thread);
    }

    return NULL;
}