#include "headers.h"

void* handle_client(void* arg) {
    int* sock_out = (int*) arg;
    cout << "Connected to socket " << *sock_out << '\n';

    char* inp[SIZE_1024];
    while(true) {
        
    }

    return NULL;
}

void* run_server(void* arg) {
    int sock_in;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("Error making socket for server.");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tracker_port);

    if(inet_pton(AF_INET, tracker_ip, &addr.sin_addr) <= 0)
        exit_error("Error with tracker ip address.");

    if(bind(sock_in, (struct sockaddr*)&addr, addrlen) < 0)
        exit_error("Error binding with socket.");

    if(listen(sock_in, BACKLOG) < 0)
        exit_error("Error listening to socket.");

    while(true) {
        int sock_out;

        if((sock_out = accept(sock_in, (struct sockaddr*)&addr, &addrlen)) < 0)
            exit_error("Error accepting client connection.");

        // dispatch thread to handle client
        pthread_t client_thread;
        if(pthread_create(&client_thread, NULL, handle_client, &sock_out) != 0) {
            cerr << "Failed to start client thread.\n";
            continue;
        }

        pthread_detach(client_thread);
    }

    return NULL;
}