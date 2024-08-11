// TODO - clean up code

#include "headers.h"
#include "cmd_defs.h"

typedef struct Download_info {
    int index;
    int chunks_num;
    string file_name;
    string peerid;
} Download_info;

void* download_handler(void* arg) {
    Download_info* df = (Download_info*)arg;
    string download_file_name = df->file_name;

    // download operations
    vector<string> download_addr = get_tokens((char*)df->peerid.c_str(), ":");
    string download_ip = download_addr[0];
    int download_port;
    download_port = stoi(download_addr[1]);

    int down_sock;
    if((down_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        panic("Error creating download socket.\n");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(download_port);

    if(inet_pton(AF_INET, download_ip.c_str(), &addr.sin_addr) <= 0)
        panic("Invalid IP address.\n");
    
    if(connect(down_sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        panic("Unable to connect to peer to download file.\n");

    if((send(down_sock, ("download " + download_file_name + " " + to_string(df->index)).c_str(), SIZE_1024, 0)) < 0)
        panic("Unable to send download command to peer.\n");

    char recv_chunk[CHUNK_SIZE+1];
    bzero(recv_chunk, CHUNK_SIZE);
    if((recv(down_sock, recv_chunk, CHUNK_SIZE, 0)) < 0)
        panic("Unable to receive chunk from peer.\n");

    cout << strlen(recv_chunk) << '\n';
    if(df->index == df->chunks_num - 1)
        recv_chunk[strlen(recv_chunk)] = '\0';

    cout << "recv: " << recv_chunk << '\n';
    // write the chunk to file (to default location)
    const char* download_path = ("./data/"+download_file_name).c_str();
    int fd;
    if((fd = open(download_path, O_RDWR | O_CREAT, S_IRWXU)) < 0)
        panic("Unable to open download file.\n");

    if(lseek(fd, df->index*CHUNK_SIZE, SEEK_SET) < 0)
        panic("Error seeking file descriptor.\n");
    
    if(write(fd, recv_chunk, strlen(recv_chunk)) < 0)
        panic("Error writing chunk to file.\n");

    // send got_chunk message to tracker to update its list of chunks available with users
    if(send(peer_sock, ("got_chunk " + download_file_name + " " + to_string(df->index)).c_str(), SIZE_1024, 0) < 0)
        panic("Error sending message to tracker.\n");

    if((recv(peer_sock, recv_chunk, CHUNK_SIZE, 0)) < 0)
        panic("Unable to receive response from tracker.\n");

    close(fd);
    close(down_sock);
    free(arg);
    return NULL;
}

// tracker response -> 200 <file_name> <peerid1> <peerid2> <peerid3>...
static int status_ok(char* payload) {
    vector<string> tracker_resp = get_tokens(payload, WHITESPACE);
    
    pthread_t download_thread[tracker_resp.size()-1];
    pthread_attr_t detached_attribute;
    pthread_attr_init(&detached_attribute);
    pthread_attr_setdetachstate(&detached_attribute, PTHREAD_CREATE_DETACHED);

    for(int i = 1; i < tracker_resp.size(); i++) {
        Download_info* df = (Download_info*)malloc(sizeof(Download_info));
        df->index = i-1;
        df->chunks_num = tracker_resp.size()-1;
        df->file_name = tracker_resp[0];
        df->peerid = tracker_resp[i];
        if(pthread_create(&download_thread[i], &detached_attribute, download_handler, (void*)df) != 0)
            panic("Error creating download thread.\n");
    }

    pthread_attr_destroy(&detached_attribute);
    return 0;
}

static int invalid_format(char* payload) {
    console_write(payload);
    return 0;
}

static int not_found(char* payload) {
    console_write(payload);
    return 0;
}

static int forbidden(char* payload) {
    console_write(payload);
    return 0;
}

static int unauthorized(char* payload) {
    console_write(payload);
    return 0;
}

static map<string, int(*)(char*)> response_handler = {
    { STATUS_OK_CODE,       status_ok },
    { INVALID_FORMAT_CODE,  invalid_format },
    { NOT_FOUND_CODE,       not_found },
    { FORBIDDEN_CODE,       forbidden },
    { UNAUTHORIZED_CODE,    unauthorized }
};

/*
    Sends downloads file request to tracker in format download_file <group_id> <file_name> <destination_path>
    Expects response from tracker as STATUS_CODE RESP
    Handles STATUS_CODE as 200(STATUS_OK), 422(INVALID_FORMAT), 404(NOT_FOUND), 403(FORBIDDEN), 409(CONFLICT_CODE), 401(UNAUTHORIZED)
*/
int download_file(char* cmd) {
    if(send(peer_sock, cmd, SIZE_1024, 0) < 0)
        panic("Error sending command to tracker.\n");
    
    char response[SIZE_1024];
    bzero(response, SIZE_1024);
    if(recv(peer_sock, response, SIZE_1024, 0) < 0)
        panic("Error receiving response from tracker.\n");
    
    char* response_code = strtok(response, WHITESPACE);
    if(!response_code) {
        console_write("Unexpected response from tracker. No response code found.\n");
        return 0;
    }
    
    char* payload = strtok(NULL, "");
    if(!payload) {
        console_write("Unexpected response from tracker. No payload found.\n");
        return 0;
    }

    if(response_handler.find(response_code) == response_handler.end()) {
        console_write("Unexpected response code from tracker.\n");
        return 0;
    }

    return response_handler[response_code](payload);
}