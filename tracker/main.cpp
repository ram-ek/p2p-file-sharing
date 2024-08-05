#include "headers.h"

pthread_mutex_t console_mtx = PTHREAD_MUTEX_INITIALIZER;
char* tracker_ip;
int tracker_port;
int tracker_sock;
map<string, string> user_list;
map<int, string> sock_to_user;
map<int, string> sock_to_peerid;
map<string, struct group*> group_list;

int main(int argc, char* argv[]) {
    if(argc < 2)
        panic("Please provide tracker socket id.\n");

    // process arguments
    process_args(argv);

    // run tracker server
    pthread_t server_thread;
    if(pthread_create(&server_thread, NULL, run_server, NULL) != 0)
        panic("Error creating server thread.\n");

    // get back interactive shell
    get_console();

    // todo - doing graceful termination
    close(tracker_sock);

    return 0;
}