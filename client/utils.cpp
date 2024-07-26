#include "headers.h"

void exit_error(string msg) {
    cout << msg << '\n';
    exit(1);
}

void process_args(char* argv[]) {

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

void send_cmd(char* cmd) {
    // do sanity checks
    

    if(send(sock_in, cmd, strlen(cmd), 0) < 0)
        exit_error("Error sending command.");
}

void connect_tracker() {
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;

    // max tries to connect to tracker
    for(int t=1; t<=MAX_CNCT_TRY; t++) {

        // iterating over trackers in order of least connected hosts;
        for(auto& [host_cnt, tracker_addrs]: tracker_list) {
            for(auto& [tracker_addr, tracker_port]: tracker_addrs) {
                addr.sin_port = htons(stoi(tracker_port));

                // try to connect to a particular tracker
                if(connect(sock_in, &addr, sizeof(addr)) >= 0) {
                    tracker_list[host_cnt+1].insert(make_pair(tracker_addr, tracker_port));
                    tracker_list[host_cnt].erase(make_pair(tracker_addr, tracker_port));
                    return;
                }
            }
        }
    }

    exit_error("Unable to connect to tracker.");
}