/* macros.h */

#pragma once

#include <stdio.h>
#include <string.h>

/* Macros */

#define debug(M, ...) \
    fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define streq(a, b)     (strcmp((a), (b)) == 0)

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
