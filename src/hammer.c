/* hammer.c: Hammer Functions */

#include "hammer.h"
#include "http.h"
#include "timestamp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <macros.h>

/**
 * Make n HTTP requests to the specified url, while writing the contents of the
 * request to the specified stream.
 *
 * Each HTTP request must be performed by a separate child process.
 *
 * @param   url         Make HTTP requests to this URL.
 * @param   stream      Write the contents of each response to this stream.
 * @param   n           The number of HTTP requests to make.
 * @return  True if all children were successful.
 **/
bool	hammer(const URL *url, FILE *stream, size_t n) {
    /* Spawn children.
     * Each child must do the following:
     *  1. Make a HTTP request to given URL.
     *  2. Time the HTTP request.
     *  3. Print the bandwidth if any data was written to stderr.
     *  4. Exit with success if the HTTP request was successful.
     **/

    double time_start = timestamp();

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid < 0) { // Error
            return false;
        } 
        if (pid == 0) { // Child
            double time_before = timestamp();
            double bytes_written = http_get(url, stream);
            double time_after = timestamp();
            double bandwidth = bytes_written / 1048576.0 / (time_after - time_before);
             
            if (bytes_written >= 0) {
                fprintf(stderr, "Bandwidth: %0.2lf MB/s\n", bandwidth);
                exit(EXIT_SUCCESS);
            } else {
                exit(EXIT_FAILURE);
            }
        }
    }
     
    /* Wait for children. */
    int all_status = 0;
    int child_status;
    for (int i = 0; i < n; i++) {
        while(wait(&child_status) < 0);
        all_status += child_status;
    }

    /* Print elapsed time to stderr. */
    double elapsed_time = timestamp() - time_start;
    fprintf(stderr, "Elapsed time: %0.2lf seconds\n", elapsed_time);
    return (all_status == 0);
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
