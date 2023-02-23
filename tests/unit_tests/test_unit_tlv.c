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

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unity.h>

#include "tlv.h"

static uint8_t g_value_in[100000];
static uint8_t g_work_buf[100000];

void setUp(void)
{
        srand(time(NULL));

        memset(g_value_in, 0xAA, sizeof(g_value_in));
        memset(g_work_buf, 0xAA, sizeof(g_work_buf));
}

void tearDown(void)
{

}

static void test_tlv_format_parse_random_value(uint32_t type, uint32_t length)
{
        int res = 0;
        uint8_t *value_out = NULL;
        uint8_t *data = NULL;
        uint32_t data_length = 0;
        uint32_t out_type = 0;
        uint32_t out_length = 0;
        uint32_t buf_length = 0;
        
        for (uint32_t i = 0; i < length; i++)
                g_value_in[i] = rand() % 256;
        
        buf_length = length + 8;

        data = g_work_buf;
        data_length = buf_length;

        res = tlv_format(&data, &data_length, type, length, g_value_in);
        TEST_ASSERT_EQUAL(TLV_RESULT_SUCCESS, res);

        data = g_work_buf;
        data_length = buf_length;
        
        res = tlv_parse(&data, &data_length, &out_type, &out_length, &value_out);
        TEST_ASSERT_EQUAL(TLV_RESULT_SUCCESS, res);

        TEST_ASSERT_EQUAL(type, out_type);
        TEST_ASSERT_EQUAL(length, out_length);
        if (length > 0)
                TEST_ASSERT_EQUAL_UINT8_ARRAY(g_value_in, value_out, length);
}

void test_tlv_format_parse_smoke_random(void)
{
        for (uint32_t n = 0; n < 300; n++) {
                uint32_t type = n;
                for (uint32_t i = 0; i < 300; i++) {
                        uint32_t length = i;
                        test_tlv_format_parse_random_value(type, length);
                }
        }
}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_tlv_format_parse_smoke_random);

        return UNITY_END();
}