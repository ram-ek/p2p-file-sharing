#include "headers.h"

int sock_in;

int main(int argc, char* argv[]) {
    if(argc < 3)
        exit_error("Please provide client socket id and tracker info.");

    // process client ip port and tracker info


    if((sock_in = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("Error creating socket.");

    get_console();

    return 0;
}