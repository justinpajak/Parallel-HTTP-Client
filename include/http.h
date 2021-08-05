/* http.h */

#pragma once

#include "url.h"

#include <stdbool.h>
#include <stdio.h>

/* Functions */

ssize_t http_get(const URL *url, FILE *stream);

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
