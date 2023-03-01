/*
MIT License

Copyright (c) 2023 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "tlv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
        int exit_code = 0;
        uint8_t *buf = NULL;

        if ((argc <= 1) || (argc % 2 == 0)) {
                puts("usage: ./format <type> <value> [<type> <value>] ... ");
                exit_code = -1;
                goto _main_end;
        }

        uint32_t estimate_length = 0;

        for (int i = 1; i < argc; i += 2) {
                estimate_length += 5;
                estimate_length += strlen(argv[i + 1]);
        }

        buf = (uint8_t *)malloc(estimate_length);
        uint32_t buf_size = estimate_length;

        uint8_t *output = buf;
        uint32_t free_space = buf_size;

        for (int i = 1; i < argc; i += 2) {
                uint32_t type = atoi(argv[i]);
                uint8_t *value = (uint8_t *)argv[i + 1];
                uint32_t length = strlen((char *)value);

                int res = tlv_format(&output, &free_space, type, length, value);
                if (res != TLV_RESULT_SUCCESS) {
                        printf("error: %d\n", res);
                        exit_code = -1;
                        goto _main_end;
                }
        }

        uint32_t out_size = buf_size - free_space;

        for (uint32_t i = 0; i < out_size; i++)
                putc(buf[i], stdout);

_main_end:

        if (buf != NULL)
                free(buf);

        return exit_code;
}
