#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define MAX_RETRY 10
#define BACKLOG 10

void exit_error(string);
void process_args(char**);
void get_console(void);
vector<string> get_tokens(char*, char*);
void connect_tracker(void);

extern int sock_in;
extern char* peer_ip;
extern int peer_port;
extern map<int, set<pair<const char*, int> > > tracker_list;

#endif