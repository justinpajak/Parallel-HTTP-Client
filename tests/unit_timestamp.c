/* unit_timestamp.c */

#include "timestamp.h"
#include "macros.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

/* Test Cases */

int test_00_timestamp() {
    for (int i = 0; i < 10; i++) {
    	double time_0 = time(NULL);
    	double time_1 = timestamp();
    	double time_d = time_1 - time_0;

    	assert(time_1 > 0 && time_d < 1.0);
    }
    return EXIT_SUCCESS;
}

/* Main execution */

int main(int argc, char *argv[]) {
    if (argc != 2) {
	fprintf(stderr, "Usage: %s NUMBER\n\n", argv[0]);
	fprintf(stderr, "Where NUMBER is right of the following:\n");
	fprintf(stderr, "    0  Test timestamp\n");
	return EXIT_FAILURE;
    }

    int number = atoi(argv[1]);
    int status = EXIT_FAILURE;

    switch (number) {
	case 0:	status = test_00_timestamp(); break;
	default:
	    fprintf(stderr, "Unknown NUMBER: %d\n", number);
	    break;
    }

    return status;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
