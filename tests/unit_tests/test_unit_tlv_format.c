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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unity.h>

#include "test_tlv.h"

#include "tlv.h"

#define DEFINE_TEST_CASE(name, expected_data, expected_result, out_size, type, length, value) \
\
void name(void)\
{\
        uint8_t test_array[66000] = {0};\
        uint8_t *data = test_array;\
        uint32_t data_length = out_size;\
\
        uint8_t expected[] = expected_data;\
        uint8_t payload[] = value;\
\
        int res = tlv_format(&data, &data_length, type, length, payload);\
\
        TEST_ASSERT_EQUAL(expected_result, res);\
        if (res == TLV_RESULT_SUCCESS) {\
                TEST_ASSERT_EQUAL(&test_array[sizeof(expected)], data);\
                TEST_ASSERT_EQUAL(out_size - sizeof(expected), data_length);\
                TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, test_array, sizeof(expected));\
        }\
}\

#define _A(...) __VA_ARGS__

DEFINE_TEST_CASE(test_tlv_format_positive_length_0,     _A({0x00}),                                 TLV_RESULT_SUCCESS, 66000, 0,     0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_1,     _A({0x10, 0xAB}),                           TLV_RESULT_SUCCESS, 66000, 0,     1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_12,    _A({0xC0, TEST_PAYLOAD_12}),                TLV_RESULT_SUCCESS, 66000, 0,    12, _A({TEST_PAYLOAD_12}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_13,    _A({0xD0, 0x00, TEST_PAYLOAD_13}),          TLV_RESULT_SUCCESS, 66000, 0,    13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_14,    _A({0xD0, 0x01, TEST_PAYLOAD_14}),          TLV_RESULT_SUCCESS, 66000, 0,    14, _A({TEST_PAYLOAD_14}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_268,   _A({0xD0, 0xFF, TEST_PAYLOAD_268}),         TLV_RESULT_SUCCESS, 66000, 0,   268, _A({TEST_PAYLOAD_268}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_269,   _A({0xE0, 0x00, 0x00, TEST_PAYLOAD_269}),   TLV_RESULT_SUCCESS, 66000, 0,   269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_270,   _A({0xE0, 0x00, 0x01, TEST_PAYLOAD_270}),   TLV_RESULT_SUCCESS, 66000, 0,   270, _A({TEST_PAYLOAD_270}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_524,   _A({0xE0, 0x00, 0xFF, TEST_PAYLOAD_524}),   TLV_RESULT_SUCCESS, 66000, 0,   524, _A({TEST_PAYLOAD_524}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_525,   _A({0xE0, 0x01, 0x00, TEST_PAYLOAD_525}),   TLV_RESULT_SUCCESS, 66000, 0,   525, _A({TEST_PAYLOAD_525}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_526,   _A({0xE0, 0x01, 0x01, TEST_PAYLOAD_526}),   TLV_RESULT_SUCCESS, 66000, 0,   526, _A({TEST_PAYLOAD_526}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_65535, _A({0xE0, 0xFE, 0xF2, TEST_PAYLOAD_65535}), TLV_RESULT_SUCCESS, 66000, 0, 65535, _A({TEST_PAYLOAD_65535}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_65536, _A({0xE0, 0xFE, 0xF3, TEST_PAYLOAD_65536}), TLV_RESULT_SUCCESS, 66000, 0, 65536, _A({TEST_PAYLOAD_65536}))
DEFINE_TEST_CASE(test_tlv_format_positive_length_65804, _A({0xE0, 0xFF, 0xFF, TEST_PAYLOAD_65804}), TLV_RESULT_SUCCESS, 66000, 0, 65804, _A({TEST_PAYLOAD_65804}))

DEFINE_TEST_CASE(test_tlv_format_positive_type_0,     _A({0x00}),             TLV_RESULT_SUCCESS, 8,     0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_1,     _A({0x01}),             TLV_RESULT_SUCCESS, 8,     1, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_12,    _A({0x0C}),             TLV_RESULT_SUCCESS, 8,    12, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_13,    _A({0x0D, 0x00}),       TLV_RESULT_SUCCESS, 8,    13, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_14,    _A({0x0D, 0x01}),       TLV_RESULT_SUCCESS, 8,    14, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_268,   _A({0x0D, 0xFF}),       TLV_RESULT_SUCCESS, 8,   268, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_269,   _A({0x0E, 0x00, 0x00}), TLV_RESULT_SUCCESS, 8,   269, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_270,   _A({0x0E, 0x00, 0x01}), TLV_RESULT_SUCCESS, 8,   270, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_524,   _A({0x0E, 0x00, 0xFF}), TLV_RESULT_SUCCESS, 8,   524, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_525,   _A({0x0E, 0x01, 0x00}), TLV_RESULT_SUCCESS, 8,   525, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_526,   _A({0x0E, 0x01, 0x01}), TLV_RESULT_SUCCESS, 8,   526, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_65535, _A({0x0E, 0xFE, 0xF2}), TLV_RESULT_SUCCESS, 8, 65535, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_65536, _A({0x0E, 0xFE, 0xF3}), TLV_RESULT_SUCCESS, 8, 65536, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_format_positive_type_65804, _A({0x0E, 0xFF, 0xFF}), TLV_RESULT_SUCCESS, 8, 65804, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_format_negative_header_1_1_0,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,   0, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_1_1_1,    _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,   0, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_1_0,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,  13, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_1_1,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,  13, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_13_1_2,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2,  13, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_1_0, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0, 269, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_1_1, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1, 269, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_1_2, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2, 269, 1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_269_1_3,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3, 269, 1, _A({0xAA}))

DEFINE_TEST_CASE(test_tlv_format_negative_header_1_13_0,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,   0, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_1_13_1,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,   0, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_1_13_2,    _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2,   0, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_13_0,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,  13, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_13_1,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,  13, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_13_2,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2,  13, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_13_13_3,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3,  13, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_13_0, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0, 269, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_13_1, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1, 269, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_13_2, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2, 269, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_13_3, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3, 269, 13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_269_13_4,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 4, 269, 13, _A({TEST_PAYLOAD_13}))

DEFINE_TEST_CASE(test_tlv_format_negative_header_1_269_0,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,   0, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_1_269_1,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,   0, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_1_269_2,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2,   0, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_1_269_3,    _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3,   0, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_269_0,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0,  13, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_269_1,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1,  13, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_269_2,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2,  13, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_13_269_3,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3,  13, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_13_269_4,   _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 4,  13, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_269_0, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 0, 269, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_269_1, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 1, 269, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_269_2, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 2, 269, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_269_3, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 3, 269, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_header_269_269_4, _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 4, 269, 269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_format_negative_value_269_269_5,  _A({0}), TLV_RESULT_ERROR_BUFFER_TOO_SMALL, 5, 269, 269, _A({TEST_PAYLOAD_269}))

void setUp(void)
{

}

void tearDown(void)
{

}

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        UNITY_BEGIN();

        RUN_TEST(test_tlv_format_positive_length_0);
        RUN_TEST(test_tlv_format_positive_length_1);
        RUN_TEST(test_tlv_format_positive_length_12);
        RUN_TEST(test_tlv_format_positive_length_13);
        RUN_TEST(test_tlv_format_positive_length_14);
        RUN_TEST(test_tlv_format_positive_length_268);
        RUN_TEST(test_tlv_format_positive_length_269);
        RUN_TEST(test_tlv_format_positive_length_270);
        RUN_TEST(test_tlv_format_positive_length_524);
        RUN_TEST(test_tlv_format_positive_length_525);
        RUN_TEST(test_tlv_format_positive_length_526);
        RUN_TEST(test_tlv_format_positive_length_65535);
        RUN_TEST(test_tlv_format_positive_length_65536);
        RUN_TEST(test_tlv_format_positive_length_65804);

        RUN_TEST(test_tlv_format_positive_type_0);
        RUN_TEST(test_tlv_format_positive_type_1);
        RUN_TEST(test_tlv_format_positive_type_12);
        RUN_TEST(test_tlv_format_positive_type_13);
        RUN_TEST(test_tlv_format_positive_type_14);
        RUN_TEST(test_tlv_format_positive_type_268);
        RUN_TEST(test_tlv_format_positive_type_269);
        RUN_TEST(test_tlv_format_positive_type_270);
        RUN_TEST(test_tlv_format_positive_type_524);
        RUN_TEST(test_tlv_format_positive_type_525);
        RUN_TEST(test_tlv_format_positive_type_526);
        RUN_TEST(test_tlv_format_positive_type_65535);
        RUN_TEST(test_tlv_format_positive_type_65536);
        RUN_TEST(test_tlv_format_positive_type_65804);

        RUN_TEST(test_tlv_format_negative_header_1_1_0);
        RUN_TEST(test_tlv_format_negative_value_1_1_1);
        RUN_TEST(test_tlv_format_negative_header_13_1_0);
        RUN_TEST(test_tlv_format_negative_header_13_1_1);
        RUN_TEST(test_tlv_format_negative_value_13_1_2);
        RUN_TEST(test_tlv_format_negative_header_269_1_0);
        RUN_TEST(test_tlv_format_negative_header_269_1_1);
        RUN_TEST(test_tlv_format_negative_header_269_1_2);
        RUN_TEST(test_tlv_format_negative_value_269_1_3);

        RUN_TEST(test_tlv_format_negative_header_1_13_0);
        RUN_TEST(test_tlv_format_negative_header_1_13_1);
        RUN_TEST(test_tlv_format_negative_value_1_13_2);
        RUN_TEST(test_tlv_format_negative_header_13_13_0);
        RUN_TEST(test_tlv_format_negative_header_13_13_1);
        RUN_TEST(test_tlv_format_negative_header_13_13_2);
        RUN_TEST(test_tlv_format_negative_value_13_13_3);
        RUN_TEST(test_tlv_format_negative_header_269_13_0);
        RUN_TEST(test_tlv_format_negative_header_269_13_1);
        RUN_TEST(test_tlv_format_negative_header_269_13_2);
        RUN_TEST(test_tlv_format_negative_header_269_13_3);
        RUN_TEST(test_tlv_format_negative_value_269_13_4);

        RUN_TEST(test_tlv_format_negative_header_1_269_0);
        RUN_TEST(test_tlv_format_negative_header_1_269_1);
        RUN_TEST(test_tlv_format_negative_header_1_269_2);
        RUN_TEST(test_tlv_format_negative_value_1_269_3);
        RUN_TEST(test_tlv_format_negative_header_13_269_0);
        RUN_TEST(test_tlv_format_negative_header_13_269_1);
        RUN_TEST(test_tlv_format_negative_header_13_269_2);
        RUN_TEST(test_tlv_format_negative_header_13_269_3);
        RUN_TEST(test_tlv_format_negative_value_13_269_4);
        RUN_TEST(test_tlv_format_negative_header_269_269_0);
        RUN_TEST(test_tlv_format_negative_header_269_269_1);
        RUN_TEST(test_tlv_format_negative_header_269_269_2);
        RUN_TEST(test_tlv_format_negative_header_269_269_3);
        RUN_TEST(test_tlv_format_negative_header_269_269_4);
        RUN_TEST(test_tlv_format_negative_value_269_269_5);

        return UNITY_END();
}
