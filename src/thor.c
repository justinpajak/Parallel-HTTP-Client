/* thor.c */

#include "hammer.h"
#include "url.h"

#include <stdlib.h>
#include <string.h>

/* Globals */

char * PROGRAM_NAME = NULL;

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: %s [options] URL\n", PROGRAM_NAME);
    fprintf(stderr, "    -n N   How many times to hammer the URL\n");
    exit(status);
}

/* Main Execution */

int main(int argc, char *argv[]) {
    /* Parse command line arguments */
    int hammerings = 1;
    int argind = 1;
    if (argc < 2) {
        usage(1);
    }
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char *arg = argv[argind++];
        switch(arg[1]) {
            case 'h':
                usage(0);
                break;
            case 'n':
                if (argc > 3) {
                    hammerings = atoi(argv[argind++]);
                }
                break;
            default:
                usage(1);
                break;
        }
    }

    /* Parse URL */
    URL *u = url_parse(argv[argind]);

    /* Hammer URL */
    if (hammer(u, stdout, hammerings)) {
        url_delete(u);
        return EXIT_SUCCESS;
    } else {
        url_delete(u);
        return EXIT_FAILURE;
    }
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
