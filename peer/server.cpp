#include "headers.h"

void* handle_peer(void* arg) {
    int sock_out = *(int*) arg;
    free(arg);
    // download file request
    char cmd[SIZE_1024];
    bzero(cmd, SIZE_1024);

    if(recv(sock_out, cmd, SIZE_1024, 0) < 0) {
        console_write("Error receiving request from peer.\n");
        close(sock_out);
        return NULL;
    }
    
    // make a command struct to process command ?
    vector<string> tokens = get_tokens(cmd, WHITESPACE);

    // check for invalid command
    if(tokens[0] != "download") {
        if(send(sock_out, "422 Invalid command format.\n", SIZE_1024, 0) < 0)
            panic("Error sending chunk to peer.\n");
        
        return NULL;
    }

    // handle download
    string file_name = tokens[1];
    long long chunk_index = stoll(tokens[2]);
    string file_path = file_paths[file_name];

    int fd;
    if((fd = open(file_path.c_str(), O_RDONLY)) < 0)
        panic("File not found.\n");

    if(lseek(fd, chunk_index*CHUNK_SIZE, SEEK_SET) < 0)
        panic("Not able to seek to desired chunk.\n");

    char chunk[CHUNK_SIZE];
    int chunk_len;
    if((chunk_len = read(fd, chunk, CHUNK_SIZE)) < 0)
        panic("Unable to read chunk");
    
    if(send(sock_out, chunk, chunk_len, 0) < 0)
        panic("Error sending chunk to peer.\n");

    close(fd);
    close(sock_out);
    return NULL;
}

void* run_server(void* arg) {
    int sock_in;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        panic("Error making socket for server.\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(peer_port);

    if(inet_pton(AF_INET, peer_ip, &addr.sin_addr) <= 0)
        panic("Error with tracker ip address.\n");

    if(bind(sock_in, (const struct sockaddr*)&addr, addrlen) < 0)
        panic("Error binding with socket.");

    if(listen(sock_in, BACKLOG) < 0)
        panic("Error listening to socket.\n");
    
    while(true) {
        int* sock_out = (int*)malloc(sizeof(int));

        if((*sock_out = accept(sock_in, (struct sockaddr*)&addr, &addrlen)) < 0) {
            console_write("Error accepting peer connection.\n");
            free(sock_out);
        }
        
        // dispatch thread to handle peer
        pthread_t peer_thread;
        pthread_attr_t detached_attribute;
        pthread_attr_init(&detached_attribute);
        pthread_attr_setdetachstate(&detached_attribute, PTHREAD_CREATE_DETACHED);

        if(pthread_create(&peer_thread, &detached_attribute, handle_peer, (void*)sock_out) != 0) {
            console_write("Failed to start peer thread.\n");
            close(*sock_out);
            free(sock_out);
        }

        pthread_attr_destroy(&detached_attribute);
    }

    return NULL;
}