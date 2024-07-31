#include "headers.h"

void* handle_peer(void* arg) {
    int* sock_out = (int*) arg;
    console_write(("\nConnected to socket " + to_string(*sock_out) + '\n').c_str());
    console_write(">> ");

    return NULL;
}

void* run_server(void* arg) {
    int sock_in;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        panic("Error making socket for server.");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(peer_port);

    if(inet_pton(AF_INET, peer_ip, &addr.sin_addr) <= 0)
        panic("Error with tracker ip address.");

    if(bind(sock_in, (const struct sockaddr*)&addr, addrlen) < 0)
        panic("Error binding with socket.");

    if(listen(sock_in, BACKLOG) < 0)
        panic("Error listening to socket.");
    
    while(true) {
        int sock_out;

        if((sock_out = accept(sock_in, (struct sockaddr*)&addr, &addrlen)) < 0)
            panic("Error accepting client connection.");

        // dispatch thread to handle client
        pthread_t client_thread;
        if(pthread_create(&client_thread, NULL, handle_peer, &sock_out) != 0) {
            console_write("Failed to start client thread.\n");
            continue;
        }

        pthread_detach(client_thread);
    }

    return NULL;
}