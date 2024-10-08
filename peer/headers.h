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
#include <fcntl.h>

using namespace std;

#define SIZE_1024 1024
#define CHUNK_SIZE 524288
#define MAX_RETRY 10
#define BACKLOG 10
#define WHITESPACE " \t\r\n\v"

void console_write(const char*);
void panic(const char*);
void process_args(char**);
void peek(char*, char*, char*);
vector<string> get_tokens(char*, char*);
void* run_server(void*);
void connect_tracker(void);
long long get_file_size(const char*);
int process_cmd(char*);
void get_console(void);

extern char* user;
extern pthread_mutex_t console_mtx;
extern int peer_sock;
extern char* peer_ip;
extern int peer_port;
extern map<int, set<pair<const char*, int> > > tracker_list;
extern vector<int> peer_connections;
extern map<string, string> file_paths;

#endif