/* url.h */

#pragma once

/* Structure */

typedef struct {
    char *host;	    /* Parsed URL host */
    char *port;	    /* Parsed URL port */
    char *path;	    /* Parsed URL path */
} URL;

/* Functions */

URL *	url_parse(const char *url_string);
void	url_delete(URL* url);

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
