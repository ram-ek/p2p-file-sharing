struct Server
{
public:
    int domain;
    int service;
    int protocol;
    unsigned long interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
};