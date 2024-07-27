#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define MAX_RETRY 10
#define BACKLOG 10
#define QUIT_CODE "500"

void exit_error(string);
void process_args(char**);
vector<string> get_tokens(char*, char*);
void* run_server(void*);
void connect_tracker(void);
void get_console(void);

extern int peer_sock;
extern char* peer_ip;
extern int peer_port;
extern map<int, set<pair<const char*, int> > > tracker_list;

#endif