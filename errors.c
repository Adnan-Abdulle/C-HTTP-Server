//
// Created by Adnan Abdulle on 2026-02-08.
//

#include "errors.h"

void file_not_found(int client){
    const char *errorHdr = "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n\r\n";

    const char *errorMsg = "404 not found\n";

    write_fully(client, errorHdr, strlen(errorHdr));
    write_fully(client, errorMsg, strlen(errorMsg));
}


void not_implemented(int client){
    const char *errorHdr = "HTTP/1.1 501 Not Implemented\r\n"
                    "Content-Type: text/plain\r\n\r\n";

    const char *errorMsg = "Method not supported\n";

    write_fully(client, errorHdr, strlen(errorHdr));
    write_fully(client, errorMsg, strlen(errorMsg));
}

