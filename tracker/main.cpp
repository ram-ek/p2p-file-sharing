#include "headers.h"

pthread_mutex_t console_mtx = PTHREAD_MUTEX_INITIALIZER;

char* tracker_ip;
int tracker_port;
int tracker_sock;
map<string, string> user_list;
map<int, string> sock_to_user;
map<int, string> sock_to_peerid;
map<string, Group*> group_list;

int main(int argc, char* argv[]) {
    if(argc < 2)
        panic("Please provide tracker socket id.\n");

    srand(time(NULL));
    // process arguments
    process_args(argv);

    // run tracker server
    pthread_t server_thread;
    pthread_attr_t detached_attribute;
    pthread_attr_init(&detached_attribute);
    pthread_attr_setdetachstate(&detached_attribute, PTHREAD_CREATE_DETACHED);
    if(pthread_create(&server_thread, &detached_attribute, run_server, NULL) != 0)
        panic("Error creating server thread.\n");

    // get back interactive shell
    get_console();

    // todo - doing graceful termination
    pthread_attr_destroy(&detached_attribute);
    close(tracker_sock);

    return 0;
}