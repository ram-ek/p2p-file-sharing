#include "client.h"

struct Client client_constructor(int domain, int service, int protocol, int port, unsigned long interface)
{
    struct Client client;
    client.domain = domain;
    client.port = port;
    client.interface = interface;
    client.socket = socket(domain, service, protocol);

    return client;
}

void connect_tracker(struct Client *client, const char* tracker_ip)
{
    struct sockaddr_in server_address;

    server_address.sin_addr.s_addr = htonl(client->interface);
    server_address.sin_family = client->domain;
    server_address.sin_port = htons(client->port);

    inet_pton(client->domain, tracker_ip, &server_address.sin_addr);
    connect(client->socket, (struct sockaddr*)&server_address, sizeof(server_address));
}

void request(struct Client *client, const char* request)
{
    if(write(client->socket, request, 1024) < 0)
    {
        perror("Error sending");
        exit(1);
    }

    char response[1024];

    read(client->socket, response, 1024);

    std::cout << response;
}

void get_tracker_ip_port(const char* trackerpath, std::string& tracker_ip, int& tracker_port)
{
    int fd = open(trackerpath, O_RDONLY);
    char buffer[1024];

    if(read(fd, buffer, 1024) == -1)
    {
        perror("Error reading tracker info");
        exit(1);
    }

    std:: string address = buffer;

    int pos = address.find(":");

    tracker_ip = address.substr(0, pos);
    tracker_port = atoi(address.substr(pos+1, address.size()).c_str());

    close(fd);
}

void get_client_ip_port(char* sock_addr, std::string& ip, int& port)
{
    std::string address = sock_addr;
    int pos = address.find(":");
    
    ip = address.substr(0, pos);
    port = atoi(address.substr(pos+1, address.size()).c_str());
}