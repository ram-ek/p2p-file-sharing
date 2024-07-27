#include "headers.h"

int sock_in;
map<int, set<pair<const char*, int> > > tracker_list;

int main(int argc, char* argv[]) {
    if(argc < 3)
        exit_error("Please provide peer socket id and tracker info.");

    // process arguments
    process_args(argv);

    // run server
    // pthread_t server_thread;
    // if(pthread_create(&server_thread, NULL, run_server, NULL) != 0)
    //     exit_error("Error creating server thread.");

    // connect to tracker
    connect_tracker();

    // get interactive shell
    get_console();

    // todo - doing graceful termination

    return 0;
}