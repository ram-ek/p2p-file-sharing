#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define BACKLOG 10
#define NO_USER "NO_USER"
#define WHITESPACE " \t\r\n\v"

struct group {
    string group_name;
    string group_owner;
    set<string> group_members;
};

void console_write(const char*);
void panic(const char*);
void process_args(char**);
void* run_server(void*);
int process_cmd(char*, int*);
void peek(char*, char*, char*);
vector<string> get_tokens(char*, char*);
void bind_user_to_port(int, string);
string get_user_from_port(int);
void remove_port_bind(int);
void get_console(void);

extern pthread_mutex_t console_mtx;
extern char* tracker_ip;
extern int tracker_port;
extern int tracker_sock;
extern map<string, string> user_list;
extern map<int, string> port_to_user;
extern map<string, struct group*> group_list;

#endif