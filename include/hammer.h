/* hammer.h */

#pragma once

#include "url.h"

#include <stdbool.h>
#include <stdio.h>

/* Functions */

bool	hammer(const URL *url, FILE *stream, size_t n);

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
