#include "headers.h"

void process_args(char* argv[]) {
    vector<string> socket_id = get_tokens(argv[1], ":");
    
    if(socket_id.size() != 2)
        panic("Invalid socket id provided.\n");

    tracker_ip = new char[socket_id[0].size() + 1];
    strcpy(tracker_ip, socket_id[0].c_str());
    tracker_port = stoi(socket_id[1]);

    // todo - store tracker details in tracker_info.txt

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

void bind_user_to_sock(int sock, string user) {
    sock_to_user[sock] = user;
}

string get_user_from_sock(int sock) {
    return sock_to_user[sock];
}

void remove_sock_bind(int sock) {
    sock_to_user.erase(sock);
}

void bind_peerid_to_sock(int sock, string peerid) {
    sock_to_peerid[sock] = peerid;
}

string get_peerid_from_sock(int sock) {
    return sock_to_peerid[sock];
}