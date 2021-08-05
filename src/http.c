/* http.c: HTTP Client Functions */

#include "http.h"
#include "macros.h"
#include "socket.h"
#include "url.h"

/** 
 * Make a HTTP request to specified URL and write the contents of the response
 * to the specified stream.
 *
 * @param   url     Make a HTTP request to this URL.
 * @param   stream  Write the contents of response to this stream.
 * @return  -1 on error, otherwise the number of bytes written.
 **/
ssize_t http_get(const URL *url, FILE *stream) {
    /* TODO: Connect to remote host and port */
    FILE *client_file = socket_dial(url->host, url->port);
    if (!client_file) {
        return -1;
    }

    /* TODO: Send request to server */
    fprintf(client_file, "GET /%s HTTP/1.0\r\n", url->path);
    fprintf(client_file, "Host: %s\r\n", url->host);
    fprintf(client_file, "\r\n");

    /* TODO: Read response status from server */
    char buffer[BUFSIZ];
    fgets(buffer, BUFSIZ, client_file);
    if (strstr(buffer, "200 OK") == NULL) {
        return -1;
    }

    /* TODO: Read response headers from server */
    char headers[BUFSIZ];
    ssize_t c_length = 0;
    while (fgets(buffer, BUFSIZ, client_file) && strlen(buffer) > 2) {
        if (strstr(buffer, "Content-Length")) {
            sscanf(buffer, "%s %zu", headers, &c_length);
        }
    }

    /* TODO: Read response body from server */
    size_t bytes = 0;
    ssize_t tot = 0;

    while ((bytes = fread(buffer, 1, BUFSIZ, client_file)) > 0) {
        tot += bytes;
        fwrite(buffer, 1, bytes, stream);
    }

    /* TODO: Close connection */
    fclose(client_file);

    /* TODO: Return number of bytes written and check if it matches Content-Length */
    if (c_length > 0 && c_length != tot) { 
        return -1;
    }
    return tot;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
