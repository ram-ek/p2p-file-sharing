#include "headers.h"
#include "client_util.cpp"
#include "server_util.cpp"

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cout << "Please provide client socket id (ip:port) & tracker info.\n";
        exit(1);
    }

    std::string tracker_ip;
    int client_port, tracker_port;
    get_tracker_ip_port(argv[2], tracker_ip, tracker_port);

    std::cout << tracker_ip << std::endl;

    struct Client client = client_constructor(AF_INET, SOCK_STREAM, 0, tracker_port, INADDR_ANY);

    get_client_ip_port(argv[1], client.ip, client_port);

    // server requests
    std::thread t(start_server, client_port, 10);

    connect_tracker(&client, tracker_ip.c_str());

    // client requests
    while(true)
    {
        printf(">> ");
        std::string cmd="";
        std::getline(std::cin, cmd);

        request(&client, cmd.c_str());
    }

    t.join();

    return 0;
}