#include "headers.h"

char* tracker_ip;
int tracker_port;
int tracker_sock;

int main(int argc, char* argv[]) {
    if(argc < 2)
        exit_error("Please provide tracker socket id.");

    // process arguments
    process_args(argv);

    // run tracker server
    pthread_t server_thread;
    if(pthread_create(&server_thread, NULL, run_server, NULL) != 0)
        exit_error("Error creating server thread.");

    // get back interactive shell
    get_console();

    return 0;
}