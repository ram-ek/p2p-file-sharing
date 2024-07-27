#include "headers.h"

bool send_cmd(char* cmd) {
    // do sanity checks
    

    if(send(peer_sock, cmd, strlen(cmd), 0) < 0)
        exit_error("Error sending command.");
}

void get_console() {
    char inp[SIZE_1024];

    while(true) {
        cout << ">> ";
        cin.getline(inp, SIZE_1024);

        vector<string> inp_tok = get_tokens(inp, " ");
        if(inp_tok.size() == 0)
            continue;

        // todo - more checks needed
        // if(inp_tok.size() == 1 && inp_tok[0] == "quit")
        //     break;

        // send commands to server for processing
        if(!send_cmd(inp))
            break;
    }
}