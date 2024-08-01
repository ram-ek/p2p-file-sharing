#include "headers.h"

void process_args(char* argv[]) {
    // set up peer ip and port
    vector<string> peer_socket_id = get_tokens(argv[1], ":");
    if(peer_socket_id.size() != 2)
        panic("Invalid socket id provided.");

    peer_ip = new char[peer_socket_id[0].size() + 1];
    strcpy(peer_ip, peer_socket_id[0].c_str());
    peer_port = stoi(peer_socket_id[1]);

    // process tracker info
    tracker_list[0].insert(make_pair("127.0.0.1", 7000));
}

void peek(char* ps, char* tok, char* deli) {
    char* s;
    char* es = ps + strlen(ps);

    s = ps;
    while(s < es && strchr(deli, *s))
        s++;
    
    char* ts = strpbrk(s, deli);
    if(!ts) {
        strncpy(tok, s, (es-s));
        tok[(es-s)] = '\0';
        return;
    }
    
    strncpy(tok, s, (ts-s));
    tok[(ts-s)] = '\0';
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

void connect_tracker() {
    // make socket
    if((peer_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        panic("Error creating socket.\n");

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;

    // max tries to connect to tracker
    for(int t=1; t<=MAX_RETRY; t++) {

        // iterating over trackers in order of least connected hosts;
        for(auto& [host_cnt, tracker_addrs]: tracker_list) {
            for(auto& [tracker_addr, tracker_port]: tracker_addrs) {
                if(inet_pton(AF_INET, tracker_addr, &addr.sin_addr) <= 0)
                    panic("Invalid IP address.\n");
                
                addr.sin_port = htons(tracker_port);

                // try to connect to a particular tracker
                if(connect(peer_sock, (struct sockaddr*)&addr, sizeof(addr)) >= 0) {
                    tracker_list[host_cnt+1].insert(make_pair(tracker_addr, tracker_port));
                    tracker_list[host_cnt].erase(make_pair(tracker_addr, tracker_port));
                    return;
                }
            }
        }
    }

    panic("Unable to connect to tracker.\n");
}