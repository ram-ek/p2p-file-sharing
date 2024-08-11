#include "headers.h"

void* handle_client(void* arg) {
    int sock_out = *(int*) arg;
    free(arg);
    // console_write(("Connected to socket " + to_string(*sock_out) + '\n').c_str());
    // console_write(">> ");
    char cmd[SIZE_1024];
    while(true) {
        bzero(cmd, SIZE_1024);
        
        if(recv(sock_out, cmd, SIZE_1024, 0) < 0) {
            console_write("Error receiving command from peer.\n");
            break;
        }
        cout << cmd << '\n';
        if(process_cmd(cmd, sock_out) != 0)
            break;
    }

    // console_write(("Disconnected to socket " + to_string(*sock_out) + '\n').c_str());
    // console_write(">> ");

    // free up the port mapping for other users
    remove_sock_bind(sock_out);
    close(sock_out);
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

        if((*sock_out = accept(tracker_sock, (struct sockaddr*)&addr, &addrlen)) < 0) {
            // check why multiple error messages popping
            // console_write("Error accepting client connection.\n");
            free(sock_out);
            continue;
        }

        // getting server peerid from peer
        char peerid[SIZE_1024];
        bzero(peerid, SIZE_1024);
        if(recv(*sock_out, peerid, SIZE_1024, 0) < 0) {
            console_write("Error receiving peerid from peer.\n");
            close(*sock_out);
            free(sock_out);
            continue;
        }

        bind_peerid_to_sock(*sock_out, peerid);
        bind_user_to_sock(*sock_out, NO_USER);
        
        // dispatch thread to handle client
        pthread_t peer_thread;
        pthread_attr_t detached_attribute;
        pthread_attr_init(&detached_attribute);
        pthread_attr_setdetachstate(&detached_attribute, PTHREAD_CREATE_DETACHED);

        if(pthread_create(&peer_thread, &detached_attribute, handle_client, (void*)sock_out) != 0) {
            console_write("Failed to start client thread.\n");
            close(*sock_out);
            free(sock_out);
        }
            
        pthread_attr_destroy(&detached_attribute);
    }

    return NULL;
}