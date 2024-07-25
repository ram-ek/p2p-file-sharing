#ifndef _HEADERS__H
#define _HEADERS__H

#include <iostream>
#include <vector>
#include <sstream>
#include <sys/socket.h>

using namespace std;

void get_console(void);
void exit_error(string);

extern int sock_in;

#endif