/* unit_socket.c */

#include "socket.h"
#include "url.h"
#include "macros.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Constants */

URL GOOD_URLS[] = {
    {.host = "google.com"        , .port = "80"},
    {.host = "weasel.h4x0r.space", .port = "9110"},
    {.host = NULL},
};

URL BAD_URLS[] = {
    {.host = "localhost"         , .port = "1000"},
    {.host = "weasel.h4x0r.space", .port = "2222"},
    {.host = NULL},
};

/* Test Classes */

int test_00_socket_dial_success() {
    for (URL* url = GOOD_URLS; url->host; url++) {
        FILE *socket_stream = socket_dial(url->host, url->port); 
        assert(socket_stream);
        fclose(socket_stream);
    }
    return EXIT_SUCCESS;
}

int test_01_socket_dial_failure() {
    for (URL* url = BAD_URLS; url->host; url++) {
        fprintf(stderr, "%s:%s\n", url->host, url->port);
        FILE *socket_stream = socket_dial(url->host, url->port); 
        assert(!socket_stream);
    }
    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test socket_dial_success\n");
        fprintf(stderr, "    1  Test socket_dial_failure\n");
        return EXIT_FAILURE;
    }   

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_socket_dial_success(); break;
        case 1:  status = test_01_socket_dial_failure(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }
    
    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
