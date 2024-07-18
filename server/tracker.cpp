#include "headers.h"
#include "defs.h"
#include "user.h"
#include "group.h"
#include "server.h"
#include "uploaded_files.h"
#include "file.h"
#include "server_utils.cpp"

std::unordered_map<std::string, User> user_list;
std::unordered_map<std::string, Group> group_list;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Please provide tracker info.");
        exit(1);
    }

    std::string tracker_ip;
    int tracker_port, backlog=10;

    get_tracker_ip_port(argv[1], tracker_ip, tracker_port);
    
    // incoming requests
    std::thread t(start_server, tracker_port, backlog);

    // do something


    t.join();

    return 0;
}