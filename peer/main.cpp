#include "headers.h"

char* user;
pthread_mutex_t console_mtx = PTHREAD_MUTEX_INITIALIZER;
int sock_in;
int peer_sock;
char* peer_ip;
int peer_port;
map<int, set<pair<const char*, int> > > tracker_list;

int main(int argc, char* argv[]) {
    if(argc < 3)
        panic("Please provide peer socket id and tracker info.\n");

    // process arguments
    process_args(argv);

    // run server
    pthread_t server_thread;
    if(pthread_create(&server_thread, NULL, run_server, NULL) != 0)
        panic("Error creating server thread.\n");

    // connect to tracker
    connect_tracker();

    // get interactive shell
    get_console();

    // todo - doing graceful termination
    close(peer_sock);

    return 0;
}