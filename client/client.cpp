#include "headers.h"

int sock_in;
map<int, set<pair<string, string> > > tracker_list;

int main(int argc, char* argv[]) {
    if(argc < 3)
        exit_error("Please provide client socket id and tracker info.");

    // process arguments
    process_args(argv);

    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("Error creating socket.");

    // connect to tracker
    // connect_tracker();

    // get interactive shell
    get_console();

    // todo - doing graceful termination
    close(sock_in);

    return 0;
}