/* unit_url.c */

#include "url.h"
#include "macros.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Constants */

typedef struct {
    char *url_string;
    URL   url;
} URLTestCase;

URLTestCase TestCases[] = {
    {"test.tld"                       , {.host = "test.tld", .port = "80", .path = ""}},
    {"test.tld:25"                    , {.host = "test.tld", .port = "25", .path = ""}},
    {"test.tld/"                      , {.host = "test.tld", .port = "80", .path = ""}},
    {"test.tld/data"                  , {.host = "test.tld", .port = "80", .path = "data"}},
    {"test.tld/path/to/data"          , {.host = "test.tld", .port = "80", .path = "path/to/data"}},
    {"test.tld:25/data"               , {.host = "test.tld", .port = "25", .path = "data"}},
    {"test.tld:25/path/to/data"       , {.host = "test.tld", .port = "25", .path = "path/to/data"}},
    {"http://test.tld"                , {.host = "test.tld", .port = "80", .path = ""}},
    {"http://test.tld:25"             , {.host = "test.tld", .port = "25", .path = ""}},
    {"http://test.tld/"               , {.host = "test.tld", .port = "80", .path = ""}},
    {"http://test.tld/data"           , {.host = "test.tld", .port = "80", .path = "data"}},
    {"http://test.tld/path/to/data"   , {.host = "test.tld", .port = "80", .path = "path/to/data"}},
    {"http://test.tld:25/data"        , {.host = "test.tld", .port = "25", .path = "data"}},
    {"http://test.tld:25/path/to/data", {.host = "test.tld", .port = "25", .path = "path/to/data"}},
    {NULL}
};

/* Test Cases */

int test_00_url_parse() {
    for (URLTestCase *tc = TestCases; tc->url_string; tc++) {
        URL *url = url_parse(tc->url_string);

        assert(url);
        assert(streq(url->host, tc->url.host));
        assert(streq(url->port, tc->url.port));
        assert(streq(url->path, tc->url.path));

        free(url->host);
        free(url->port);
        free(url->path);
        free(url);
    }

    return EXIT_SUCCESS;
}

int test_01_url_delete() {
    for (URLTestCase *tc = TestCases; tc->url_string; tc++) {
        URL *url = url_parse(tc->url_string);

        assert(url);
        assert(streq(url->host, tc->url.host));
        assert(streq(url->port, tc->url.port));
        assert(streq(url->path, tc->url.path));

        url_delete(url);
    }
    return EXIT_SUCCESS;
}

/* Main Execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
        fprintf(stderr, "Where NUMBER is right of the following:\n");
        fprintf(stderr, "    0  Test url_parse\n");
        fprintf(stderr, "    1  Test url_delete\n");
        return EXIT_FAILURE;
    }   

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
        case 0:  status = test_00_url_parse(); break;
        case 1:  status = test_01_url_delete(); break;
        default: fprintf(stderr, "Unknown NUMBER: %d\n", number); break;
    }
    
    return status;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
