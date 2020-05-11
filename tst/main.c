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

#include <execinfo.h>
#include <string.h>
#include "traceback.h"
#include "nala.h"

TEST(test_traceback_print)
{
    CAPTURE_OUTPUT(output, errput) {
        traceback_print(NULL, NULL, NULL, NULL);
    }

    ASSERT_SUBSTRING(output, "Traceback (most recent call first):\n  ");
}

TEST(test_traceback_print_prefix)
{
    CAPTURE_OUTPUT(output, errput) {
        traceback_print("XXX", NULL, NULL, NULL);
    }

    ASSERT_SUBSTRING(output,
                     "XXXTraceback (most recent call first):\n"
                     "XXX ");
}

TEST(test_traceback_print_header)
{
    CAPTURE_OUTPUT(output, errput) {
        traceback_print(NULL, "My header:", NULL, NULL);
    }

    ASSERT_SUBSTRING(output,
                     "My header:\n"
                     " ");
}

static bool fum_bar_filter(void *arg_p, const char *line_p)
{
    (void)arg_p;

    if (strncmp(line_p, "fum at ", 7) == 0) {
        return (false);
    }

    if (strncmp(line_p, "fie at ", 7) == 0) {
        return (false);
    }

    if (strncmp(line_p, "bar at ", 7) == 0) {
        return (false);
    }

    return (true);
}

void foo(void)
{
    traceback_print(NULL, NULL, fum_bar_filter, NULL);
}

void bar(void)
{
    foo();
}

void fie(void)
{
    bar();
}

void fum(void)
{
    fie();
}

void fam(void)
{
    fum();
}

TEST(test_traceback_skip_filter)
{
    CAPTURE_OUTPUT(output, errput) {
        fam();
    }

    ASSERT_NOT_SUBSTRING(output, "fam");
    ASSERT_SUBSTRING(output, "fum");
    ASSERT_SUBSTRING(output, "fie");
    ASSERT_SUBSTRING(output, "bar");
    ASSERT_NOT_SUBSTRING(output, "foo");
}

TEST(test_traceback_format)
{
    char *string_p;
    int depth;
    void *addresses[32];

    depth = backtrace(&addresses[0], 32);
    string_p = traceback_format(addresses, depth, NULL, NULL, NULL, NULL);

    ASSERT_SUBSTRING(string_p,
                     "Traceback (most recent call first):\n"
                     "  ");
}
