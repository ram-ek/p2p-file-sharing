#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define BACKLOG 10
#define QUIT_CODE "500"

void exit_error(const char*);
void process_args(char**);
void* run_server(void*);
vector<string> get_tokens(char*, char*);
void get_console(void);

extern char* tracker_ip;
extern int tracker_port;
extern int tracker_sock;

#endif