#include "headers.h"

void exit_error(const char* msg) {
    cerr << msg << '\n';
    exit(1);
}

void process_args(char* argv[]) {
    vector<string> socket_id = get_tokens(argv[1], ":");
    
    if(socket_id.size() != 2)
        exit_error("Invalid socket id provided.");

    tracker_ip = new char[socket_id[0].size() + 1];
    strcpy(tracker_ip, socket_id[0].c_str());
    tracker_port = stoi(socket_id[1]);

    // todo - store tracker details in tracker_info.txt
}

void* handle_client(void* arg) {
    int* sock_out = (int*) arg;
    cout << "Connected to socket " << *sock_out << '\n';

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

    if(bind(sock_in, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        exit_error("Error binding with socket.");

    if(listen(sock_in, BACKLOG) < 0)
        exit_error("Error listening to socket.");
    cerr << tracker_ip << '\n';
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

vector<string> get_tokens(char* s, char* deli) {
    char *tok = strtok(s, deli);
    vector<string> tokens;

    while(tok != NULL) {
        tokens.push_back(tok);
        tok = strtok(NULL, deli);
    }

    return tokens;
}