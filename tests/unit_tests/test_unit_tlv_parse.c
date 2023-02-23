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


#define DEFINE_TEST_CASE(name, input_data_size, input_data, expected_result, expected_type, expected_length, expected_value) \
\
void name(void)\
{\
        uint32_t type = 0;\
        uint32_t length = 0;\
        uint8_t *value = NULL;\
        uint8_t test_array[] = input_data;\
        uint8_t expected_array[] = expected_value;\
\
        uint8_t *data = test_array;\
        uint32_t data_length = input_data_size;\
\
        int res = tlv_parse(&data, &data_length, &type, &length, &value);\
\
        TEST_ASSERT_EQUAL(expected_result, res);\
        if (res == TLV_RESULT_SUCCESS) {\
                TEST_ASSERT_EQUAL(expected_type, type);\
                TEST_ASSERT_EQUAL(expected_length, length);\
                if (expected_length > 0) {\
                        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_array, value, expected_length);\
                }\
                TEST_ASSERT_EQUAL(&test_array[input_data_size], data);\
                TEST_ASSERT_EQUAL(0, data_length);\
        }\
}\

#define _A(...) __VA_ARGS__

DEFINE_TEST_CASE(test_tlv_parse_positive_length_0,         1, _A({0x00}),                                 TLV_RESULT_SUCCESS, 0,     0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_1,         2, _A({0x10, 0xAA}),                           TLV_RESULT_SUCCESS, 0,     1, _A({0xAA}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_12,       13, _A({0xC0, TEST_PAYLOAD_12}),                TLV_RESULT_SUCCESS, 0,    12, _A({TEST_PAYLOAD_12}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_13,       15, _A({0xD0, 0x00, TEST_PAYLOAD_13}),          TLV_RESULT_SUCCESS, 0,    13, _A({TEST_PAYLOAD_13}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_14,       16, _A({0xD0, 0x01, TEST_PAYLOAD_14}),          TLV_RESULT_SUCCESS, 0,    14, _A({TEST_PAYLOAD_14}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_268,     270, _A({0xD0, 0xFF, TEST_PAYLOAD_268}),         TLV_RESULT_SUCCESS, 0,   268, _A({TEST_PAYLOAD_268}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_269,     272, _A({0xE0, 0x00, 0x00, TEST_PAYLOAD_269}),   TLV_RESULT_SUCCESS, 0,   269, _A({TEST_PAYLOAD_269}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_270,     273, _A({0xE0, 0x00, 0x01, TEST_PAYLOAD_270}),   TLV_RESULT_SUCCESS, 0,   270, _A({TEST_PAYLOAD_270}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_524,     527, _A({0xE0, 0x00, 0xFF, TEST_PAYLOAD_524}),   TLV_RESULT_SUCCESS, 0,   524, _A({TEST_PAYLOAD_524}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_525,     528, _A({0xE0, 0x01, 0x00, TEST_PAYLOAD_525}),   TLV_RESULT_SUCCESS, 0,   525, _A({TEST_PAYLOAD_525}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_526,     529, _A({0xE0, 0x01, 0x01, TEST_PAYLOAD_526}),   TLV_RESULT_SUCCESS, 0,   526, _A({TEST_PAYLOAD_526}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_65535, 65538, _A({0xE0, 0xFE, 0xF2, TEST_PAYLOAD_65535}), TLV_RESULT_SUCCESS, 0, 65535, _A({TEST_PAYLOAD_65535}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_65536, 65539, _A({0xE0, 0xFE, 0xF3, TEST_PAYLOAD_65536}), TLV_RESULT_SUCCESS, 0, 65536, _A({TEST_PAYLOAD_65536}))
DEFINE_TEST_CASE(test_tlv_parse_positive_length_65804, 65807, _A({0xE0, 0xFF, 0xFF, TEST_PAYLOAD_65804}), TLV_RESULT_SUCCESS, 0, 65804, _A({TEST_PAYLOAD_65804}))

DEFINE_TEST_CASE(test_tlv_parse_positive_type_0,     1, _A({0x00}),             TLV_RESULT_SUCCESS,     0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_1,     1, _A({0x01}),             TLV_RESULT_SUCCESS,     1, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_12,    1, _A({0x0C}),             TLV_RESULT_SUCCESS,    12, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_13,    2, _A({0x0D, 0x00}),       TLV_RESULT_SUCCESS,    13, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_14,    2, _A({0x0D, 0x01}),       TLV_RESULT_SUCCESS,    14, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_268,   2, _A({0x0D, 0xFF}),       TLV_RESULT_SUCCESS,   268, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_269,   3, _A({0x0E, 0x00, 0x00}), TLV_RESULT_SUCCESS,   269, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_270,   3, _A({0x0E, 0x00, 0x01}), TLV_RESULT_SUCCESS,   270, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_524,   3, _A({0x0E, 0x00, 0xFF}), TLV_RESULT_SUCCESS,   524, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_525,   3, _A({0x0E, 0x01, 0x00}), TLV_RESULT_SUCCESS,   525, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_526,   3, _A({0x0E, 0x01, 0x01}), TLV_RESULT_SUCCESS,   526, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_65535, 3, _A({0x0E, 0xFE, 0xF2}), TLV_RESULT_SUCCESS, 65535, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_65536, 3, _A({0x0E, 0xFE, 0xF3}), TLV_RESULT_SUCCESS, 65536, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_positive_type_65804, 3, _A({0x0E, 0xFF, 0xFF}), TLV_RESULT_SUCCESS, 65804, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_parse_negative_length_value_1,      1, _A({0x10}),                               TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_length_header_13,    1, _A({0xD0}),                               TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_length_value_13,    14, _A({0xD0, 0x00, TEST_PAYLOAD_12}),        TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_length_header_269,   2, _A({0xE0, 0x00}),                         TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_length_value_269,  271, _A({0xE0, 0x00, 0x00, TEST_PAYLOAD_268}), TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_parse_negative_type_header_13,  1, _A({0x0D}),             TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_type_value_13,   2, _A({0x1D, 0x00}),       TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_type_header_269, 2, _A({0x0E, 0x00}),       TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_type_value_269,  3, _A({0x1E, 0x00, 0x00}), TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_13_13_0,   1, _A({0xDD}),                         TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_13_13_1,   2, _A({0xDD, 0x00}),                   TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_value_13_13,      3, _A({0xDD, 0x00, 0x00}),             TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_13_269_0,  1, _A({0xDE}),                         TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_13_269_1,  2, _A({0xDE, 0x00}),                   TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_13_269_2,  3, _A({0xDE, 0x00, 0x00}),             TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_value_13_269,     4, _A({0xDE, 0x00, 0x00, 0x00}),       TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_13_0,  1, _A({0xED}),                         TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_13_1,  2, _A({0xED, 0x00}),                   TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_13_2,  3, _A({0xED, 0x00, 0x00}),             TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_value_269_13,     4, _A({0xED, 0x00, 0x00, 0x00}),       TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_269_0, 1, _A({0xEE}),                         TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_269_1, 2, _A({0xEE, 0x00}),                   TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_269_2, 3, _A({0xEE, 0x00, 0x00}),             TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_header_269_269_3, 4, _A({0xEE, 0x00, 0x00, 0x00}),       TLV_RESULT_ERROR_INCOMPLETED_FRAME, 0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_mix_value_269_269,    5, _A({0xEE, 0x00, 0x00, 0x00, 0x00}), TLV_RESULT_ERROR_INCOMPLETED_FRAME,  0, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_parse_negative_reserved_length, 1, _A({0xF0}), TLV_RESULT_ERROR_RESERVED_VALUE,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_reserved_type,   1, _A({0x0F}), TLV_RESULT_ERROR_RESERVED_VALUE,  0, 0, _A({0}))
DEFINE_TEST_CASE(test_tlv_parse_negative_reserved_mix,    1, _A({0xFF}), TLV_RESULT_ERROR_RESERVED_VALUE,  0, 0, _A({0}))

DEFINE_TEST_CASE(test_tlv_parse_negative_no_header,       0, _A({0}), TLV_RESULT_ERROR_NO_HEADER,  0, 0, _A({0}))


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

        RUN_TEST(test_tlv_parse_positive_length_0);
        RUN_TEST(test_tlv_parse_positive_length_1);
        RUN_TEST(test_tlv_parse_positive_length_12);
        RUN_TEST(test_tlv_parse_positive_length_13);
        RUN_TEST(test_tlv_parse_positive_length_14);
        RUN_TEST(test_tlv_parse_positive_length_268);
        RUN_TEST(test_tlv_parse_positive_length_269);
        RUN_TEST(test_tlv_parse_positive_length_270);
        RUN_TEST(test_tlv_parse_positive_length_524);
        RUN_TEST(test_tlv_parse_positive_length_525);
        RUN_TEST(test_tlv_parse_positive_length_526);
        RUN_TEST(test_tlv_parse_positive_length_65535);
        RUN_TEST(test_tlv_parse_positive_length_65536);
        RUN_TEST(test_tlv_parse_positive_length_65804);

        RUN_TEST(test_tlv_parse_positive_type_0);
        RUN_TEST(test_tlv_parse_positive_type_1);
        RUN_TEST(test_tlv_parse_positive_type_12);
        RUN_TEST(test_tlv_parse_positive_type_13);
        RUN_TEST(test_tlv_parse_positive_type_14);
        RUN_TEST(test_tlv_parse_positive_type_268);
        RUN_TEST(test_tlv_parse_positive_type_269);
        RUN_TEST(test_tlv_parse_positive_type_270);
        RUN_TEST(test_tlv_parse_positive_type_524);
        RUN_TEST(test_tlv_parse_positive_type_525);
        RUN_TEST(test_tlv_parse_positive_type_526);
        RUN_TEST(test_tlv_parse_positive_type_65535);
        RUN_TEST(test_tlv_parse_positive_type_65536);
        RUN_TEST(test_tlv_parse_positive_type_65804);

        RUN_TEST(test_tlv_parse_negative_length_value_1);
        RUN_TEST(test_tlv_parse_negative_length_header_13);
        RUN_TEST(test_tlv_parse_negative_length_value_13);
        RUN_TEST(test_tlv_parse_negative_length_header_269);
        RUN_TEST(test_tlv_parse_negative_length_value_269);

        RUN_TEST(test_tlv_parse_negative_type_header_13);
        RUN_TEST(test_tlv_parse_negative_type_value_13);
        RUN_TEST(test_tlv_parse_negative_type_header_269);
        RUN_TEST(test_tlv_parse_negative_type_value_269);

        RUN_TEST(test_tlv_parse_negative_mix_header_13_13_0);
        RUN_TEST(test_tlv_parse_negative_mix_header_13_13_1);
        RUN_TEST(test_tlv_parse_negative_mix_value_13_13);
        RUN_TEST(test_tlv_parse_negative_mix_header_13_269_0);
        RUN_TEST(test_tlv_parse_negative_mix_header_13_269_1);
        RUN_TEST(test_tlv_parse_negative_mix_header_13_269_2);
        RUN_TEST(test_tlv_parse_negative_mix_value_13_269);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_13_0);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_13_1);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_13_2);
        RUN_TEST(test_tlv_parse_negative_mix_value_269_13);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_269_0);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_269_1);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_269_2);
        RUN_TEST(test_tlv_parse_negative_mix_header_269_269_3);
        RUN_TEST(test_tlv_parse_negative_mix_value_269_269);

        RUN_TEST(test_tlv_parse_negative_reserved_length);
        RUN_TEST(test_tlv_parse_negative_reserved_type);
        RUN_TEST(test_tlv_parse_negative_reserved_mix);

        RUN_TEST(test_tlv_parse_negative_no_header);

        return UNITY_END();
}