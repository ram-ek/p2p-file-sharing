#include "server.h"


Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog)
{
    Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_addr.s_addr = htonl(interface);
    server.address.sin_family = domain;
    server.address.sin_port = htons(port);

    if((server.socket = socket(domain, service, protocol)) == -1)
    {
        perror("Error making server socket");
        exit(1);
    }

    if((bind(server.socket, (struct sockaddr*) &server.address, sizeof(server.address))) < 0)
    {
        perror("Error binding server socket");
        exit(1);
    }

    if((listen(server.socket, server.backlog)) < 0)
    {
        perror("Error listening from server socket");
        exit(1);
    }

    return server;
}

void* handle_incoming_request(void* client)
{
    char* request[1024];
    memset(request, 0, 1024);

    if(read(*(int*)client, request, 1024) < 0)
    {
        perror("Error reading client request");
        exit(1);
    }

    return NULL;
}

void start_server(int port, int backlog)
{
    Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, backlog);
    struct sockaddr *address = (struct sockaddr *)&server.address;
    socklen_t address_length = (socklen_t) sizeof(server.address);

    while(true)
    {
        int client = accept(server.socket, address, &address_length);

        pthread_t in_client;
        pthread_create(&in_client, NULL, handle_incoming_request, &client);
    }

    close(server.socket);
}