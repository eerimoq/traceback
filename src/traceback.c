/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file is part of the traceback project.
 */

#include <stdio.h>
#include <stdint.h>
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include "traceback.h"
#include "subprocess.h"

#define DEPTH_MAX 100

static void *fixaddr(void *address_p)
{
    return ((void *)(((uintptr_t)address_p) - 1));
}

static bool is_traceback_line(const char *line_p)
{
    if (strncmp(line_p, "traceback_print at ", 19) == 0) {
        return (true);
    }

    if (strncmp(line_p, "traceback_string at ", 20) == 0) {
        return (true);
    }

    return (false);
}

static char *strip_discriminator(char *line_p)
{
    char *discriminator_p;

    discriminator_p = strstr(line_p, " (discriminator");

    if (discriminator_p != NULL) {
        discriminator_p[0] = '\n';
        discriminator_p[1] = '\0';
    }

    return (line_p);
}

static bool is_before_ignore_function(const char *line_p,
                                      const char *function_p,
                                      bool *before_function_p)
{
    if (function_p == NULL) {
        *before_function_p = false;
    }

    if (*before_function_p) {
        if (strncmp(line_p, function_p, strlen(function_p)) == 0) {
            *before_function_p = false;
        }
    }

    return (*before_function_p);
}

char *traceback_format(const char *ignore_until_function_p,
                       const char *prefix_p,
                       void **buffer_pp,
                       int depth)
{
    char exe[256];
    char command[384];
    ssize_t size;
    int i;
    FILE *stream_p;
    size_t stream_size;
    struct subprocess_result_t *result_p;
    char *string_p;
    bool before_function;

    if (prefix_p == NULL) {
        prefix_p = "";
    }

    size = readlink("/proc/self/exe", &exe[0], sizeof(exe) - 1);

    if (size == -1) {
        return (NULL);
    }

    exe[size] = '\0';

    stream_p = open_memstream(&string_p, &stream_size);

    if (stream_p == NULL) {
        return (NULL);
    }

    before_function = true;
    fprintf(stream_p, "%sTraceback (most recent call last):\n", prefix_p);

    for (i = (depth - 1); i >= 0; i--) {
        snprintf(&command[0],
                 sizeof(command),
                 "addr2line -f -p -e %s %p",
                 &exe[0],
                 fixaddr(buffer_pp[i]));

        result_p = subprocess_exec_output(&command[0]);

        if (result_p->exit_code != 0) {
            subprocess_result_free(result_p);
            continue;
        }

        if (is_traceback_line(result_p->stdout.buf_p)) {
            subprocess_result_free(result_p);
            continue;
        }

        if (is_before_ignore_function(result_p->stdout.buf_p,
                                      ignore_until_function_p,
                                      &before_function)) {
            subprocess_result_free(result_p);
            continue;
        }

        fprintf(stream_p, "%s  ", prefix_p);
        fwrite(strip_discriminator(result_p->stdout.buf_p),
               1,
               strlen(result_p->stdout.buf_p),
               stream_p);
        subprocess_result_free(result_p);
    }

    fclose(stream_p);

    return (string_p);
}

char *traceback_string(const char *ignore_until_function_p,
                       const char *prefix_p)
{
    int depth;
    void *addresses[DEPTH_MAX];

    depth = backtrace(&addresses[0], DEPTH_MAX);

    return (traceback_format(prefix_p, ignore_until_function_p, addresses, depth));
}

void traceback_print(const char *ignore_until_function_p,
                     const char *prefix_p)
{
    char *string_p;

    string_p = traceback_string(prefix_p, ignore_until_function_p);
    printf("%s", string_p);
    free(string_p);
}
