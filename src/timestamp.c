/* timestamp.c: Timestamp Functions */

#include "timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 **/
double timestamp() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0) {
        return (double)time(NULL); 
    }
    return (double)(t.tv_sec + (t.tv_usec / 1000000.0));
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
