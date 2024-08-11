#include "headers.h"

pthread_mutex_t console_mtx = PTHREAD_MUTEX_INITIALIZER;

char* user;
int sock_in;
int peer_sock;
char* peer_ip;
int peer_port;
map<int, set<pair<const char*, int> > > tracker_list;
vector<int> peer_connections;
map<string, string> file_paths;

int main(int argc, char* argv[]) {
    if(argc < 3)
        panic("Please provide peer socket id and tracker info.\n");

    // process arguments
    process_args(argv);

    // run server
    pthread_t server_thread;
    pthread_attr_t detached_attribute;
    pthread_attr_init(&detached_attribute);
    pthread_attr_setdetachstate(&detached_attribute, PTHREAD_CREATE_DETACHED);
    
    if(pthread_create(&server_thread, &detached_attribute, run_server, NULL) != 0)
        panic("Error creating server thread.\n");

    // connect to tracker
    connect_tracker();

    // get interactive shell
    get_console();

    // todo - doing graceful termination
    pthread_attr_destroy(&detached_attribute);
    close(peer_sock);

    return 0;
}