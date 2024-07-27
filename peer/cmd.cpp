#include "headers.h"

void get_console() {
    char inp[SIZE_1024];

    while(true) {
        cout << ">> ";
        cin.getline(inp, SIZE_1024);

        vector<string> inp_tok = get_tokens(inp, " ");
        if(inp_tok.size() == 0)
            continue;

        // todo - more checks needed
        if(inp_tok.size() == 1 && inp_tok[0] == "quit")
            break;

        // send commands to server for processing
        // send_cmd(inp);
    }
}