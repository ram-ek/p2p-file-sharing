#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define SIZE_1024 1024
#define BACKLOG 10
#define WHITESPACE " \t\r\n\v"

void console_write(const char*);
void panic(const char*);
void process_args(char**);
void* run_server(void*);
int processcmd(char*, int*);
extern char* peek(char*);
vector<string> get_tokens(char*, char*);
void get_console(void);

extern pthread_mutex_t console_mtx;
extern char* tracker_ip;
extern int tracker_port;
extern int tracker_sock;
extern map<string, string> user_list;

#endif