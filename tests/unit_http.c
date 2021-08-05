/* unit_http.c */

#include "http.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Constants */

URL GOOD_URLS[] = {
    {.host = "example.com"    , .port = "80"  , .path = ""},
    {.host = "httpforever.com", .port = "80"  , .path = ""},
    {.host = "h4x0r.space"    , .port = "9898", .path = ""},
    {.host = "h4x0r.space"    , .port = "9898", .path = "loki.py"},
    {.host = "h4x0r.space"    , .port = "9898", .path = "walden.txt"},
    {.host = NULL},
};

URL BAD_URLS[] = {
    {.host = "google.com"     , .port = "80"  , .path = ""},
    {.host = "h4x0r.space"    , .port = "80"  , .path = ""},
    {.host = "h4x0r.space"    , .port = "9898", .path = "404"},
    {.host = NULL},
};

/* Test Cases */

int test_00_http_get_success() {
    for (URL* url = GOOD_URLS; url->host; url++) {
        char temp_path[] = "test.XXXXXX";
        int  temp_fd = mkstemp(temp_path);
        if (temp_fd < 0) {
            fprintf(stderr, "mkstemp: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        FILE* temp_file = fdopen(temp_fd, "w");
        if (!temp_file) {
            fprintf(stderr, "fdopen: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        
        assert(http_get(url, temp_file) >= 0);
        fclose(temp_file);

        char command[BUFSIZ];
        sprintf(command, "curl -s %s:%s/%s | diff - %s", url->host, url->port, url->path, temp_path);
        assert(WEXITSTATUS(system(command)) == 0);
        unlink(temp_path);
    }

    return EXIT_SUCCESS;
}

int test_01_http_get_failure() {
    for (URL* url = BAD_URLS; url->host; url++) {
        assert(http_get(url, stdout) < 0);
    }
    
    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test http_get_success\n");
        fprintf(stderr, "    1  Test http_get_failure\n");
        return EXIT_FAILURE;
    }   

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_http_get_success(); break;
        case 1:  status = test_01_http_get_failure(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }
    
    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
