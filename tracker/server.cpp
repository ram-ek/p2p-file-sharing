#include "headers.h"

void* handle_client(void* arg) {
    int* sock_out = (int*) arg;
    cout << "Connected to socket " << *sock_out << '\n';

    char inp[SIZE_1024];
    char response[SIZE_1024];
    while(true) {
        bzero(inp, SIZE_1024);
        if(recv(*sock_out, inp, SIZE_1024, 0) < 0)
            exit_error("Error receiving command from peer.");

        vector<string> cmd = get_tokens(inp, " ");
        if(cmd.size() == 1 && cmd[0] == "quit") {
            if(send(*sock_out, QUIT_CODE, SIZE_1024, 0) < 0)
                exit_error("Error sending response to peer.");

            break;
        }
        else {
            strcpy(response, cmd[0].c_str());
            if(send(*sock_out, response, SIZE_1024, 0) < 0)
                exit_error("Error sending response to peer.");
        }
    }

    return NULL;
}

void* run_server(void* arg) {
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if((tracker_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("Error making socket for server.");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(tracker_port);

    if(inet_pton(AF_INET, tracker_ip, &addr.sin_addr) <= 0)
        exit_error("Error with tracker ip address.");

    if(bind(tracker_sock, (struct sockaddr*)&addr, addrlen) < 0)
        exit_error("Error binding with socket.");

    if(listen(tracker_sock, BACKLOG) < 0)
        exit_error("Error listening to socket.");

    while(true) {
        int sock_out;

        if((sock_out = accept(tracker_sock, (struct sockaddr*)&addr, &addrlen)) < 0)
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