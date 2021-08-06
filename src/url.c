/* url.c: URL Structure Functions */

#include "url.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */

#define	HOST_DELIMITER  "://"
#define	PATH_DELIMITER	'/'
#define PORT_DELIMITER	':'

/**
 * Parse URL string into separate host, port, and path components.
 *
 * @param   url_string	    URL string to parse.
 * @return  Parsed URL structure.
 **/
URL*	url_parse(const char *url_string) {
    /* Copy data to local buffer */
    char buffer[BUFSIZ];
    strcpy(buffer, url_string); 

    /* Skip scheme to host */
    char *host = strstr(buffer, HOST_DELIMITER);
    if (host) {
        host += strlen(HOST_DELIMITER);
    } else {
        host = buffer;
    }
    
    /* Split host:port from path */
    char *path = strchr(host, PATH_DELIMITER);
    if (path) {
        *path = 0;
        path += 1;
    } else {
        path = "";
    }

    /* Split host and port */
    char *port = strchr(host, PORT_DELIMITER);
    if (!port) {
        port = "80";
    } else {
        *port = 0;
        port += 1;
    }

    /* Allocate URL structure */
    URL *u = (URL*)malloc(sizeof(URL));
    if (!u) {
        return NULL;
    }

    /* Copy components to URL structure */
    u->host = strdup(host);
    u->port = strdup(port);
    u->path = strdup(path);
    
    return u;
}

/**
 * Deallocate URL structure.
 *
 * @param   url     URL structure to deallocate.
 **/
void	url_delete(URL *url) {
    /* Release components and url */
    free(url->host);
    free(url->port);
    free(url->path);
    free(url);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
