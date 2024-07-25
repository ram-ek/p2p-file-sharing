#include "headers.h"

void get_console() {
    string inp, tok;

    while(true) {
        cout << ">> ";
        getline(cin, inp);

        stringstream ss(inp);
        vector<string> inp_tok;

        while(ss >> tok)
            inp_tok.push_back(tok);

        if(inp_tok.size() == 0)
            continue;

        if(inp_tok[0] == "quit")
            break;

        // do sanity checks
        

        // send commands to server for processing
    }
}