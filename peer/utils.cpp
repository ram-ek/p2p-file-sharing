#include "headers.h"

void exit_error(string msg) {
    cerr << msg << '\n';
    exit(1);
}

void process_args(char* argv[]) {
    // set up peer ip and port
    vector<string> peer_socket_id = get_tokens(argv[1], ":");
    if(peer_socket_id.size() != 2)
        exit_error("Invalid socket id provided.");

    char* peer_ip = new char[peer_socket_id[0].size() + 1];
    strcpy(peer_ip, peer_socket_id[0].c_str());
    int peer_port = stoi(peer_socket_id[1]);

    // process tracker info
    tracker_list[0].insert(make_pair("127.0.0.1", 7000));
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

// void send_cmd(char* cmd) {
//     // do sanity checks
    

//     if(send(sock_in, cmd, strlen(cmd), 0) < 0)
//         exit_error("Error sending command.");
// }

void* handle_peer(void* arg) {

}

void* run_server(void* arg) {
    // int sock_in;
    // struct sockaddr_in addr;
    // socklen_t addrlen = sizeof(addr);

    // if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    //     exit_error("Error making socket for server.");

    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(tracker_port);

    // if(inet_pton(AF_INET, tracker_ip, &addr.sin_addr) <= 0)
    //     exit_error("Error with tracker ip address.");

    // if(bind(sock_in, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    //     exit_error("Error binding with socket.");

    // if(listen(sock_in, BACKLOG) < 0)
    //     exit_error("Error listening to socket.");
    
    // while(true) {
    //     int sock_out;

    //     if((sock_out = accept(sock_in, (struct sockaddr*)&addr, &addrlen)) < 0)
    //         exit_error("Error accepting client connection.");

    //     // dispatch thread to handle client
    //     pthread_t client_thread;
    //     if(pthread_create(&client_thread, NULL, handle_peer, &sock_out) != 0) {
    //         cerr << "Failed to start client thread.\n";
    //         continue;
    //     }

    //     pthread_detach(client_thread);
    // }

    return NULL;
}

void connect_tracker() {
    // make socket
    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("Error creating socket.");
    
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;

    // max tries to connect to tracker
    for(int t=1; t<=MAX_RETRY; t++) {

        // iterating over trackers in order of least connected hosts;
        for(auto& [host_cnt, tracker_addrs]: tracker_list) {
            for(auto& [tracker_addr, tracker_port]: tracker_addrs) {
                if(inet_pton(AF_INET, tracker_addr, &addr.sin_addr) <= 0)
                    exit_error("Invalid IP address.");
                
                addr.sin_port = htons(tracker_port);

                // try to connect to a particular tracker
                if(connect(sock_in, (struct sockaddr*)&addr, sizeof(addr)) >= 0) {
                    tracker_list[host_cnt+1].insert(make_pair(tracker_addr, tracker_port));
                    tracker_list[host_cnt].erase(make_pair(tracker_addr, tracker_port));
                    return;
                }
            }
        }
    }

    exit_error("Unable to connect to tracker.");
}