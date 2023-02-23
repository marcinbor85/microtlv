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

#include <stddef.h>
#include <assert.h>

static int parse_header(uint32_t *value, uint8_t **data, uint32_t *data_length, uint8_t header)
{
        assert(value != NULL);
        assert(data != NULL);
        assert(data_length != NULL);

        if (header < TLV_HEADER_VALUE_1) {
                *value = header;

        } else if (header == TLV_HEADER_VALUE_1) {
                if (*data_length < 1)
                        return TLV_RESULT_ERROR_INCOMPLETED_FRAME;

                *value = (uint32_t)(*data)[0];
                *value += TLV_HEADER_VALUE_1;
                *data = *data + 1;
                *data_length = *data_length - 1;

        } else if (header == TLV_HEADER_VALUE_2) {
                if (*data_length < 2)
                        return TLV_RESULT_ERROR_INCOMPLETED_FRAME;

                *value = ((((uint32_t)(*data)[0]) << 8) & 0xFF00) | (((uint32_t)(*data)[1]) & 0x00FF);
                *value += 0x0100 + TLV_HEADER_VALUE_1;
                *data = *data + 2;
                *data_length = *data_length - 2;

        } else {
                return TLV_RESULT_ERROR_RESERVED_VALUE;

        }

        return TLV_RESULT_SUCCESS;
}

int tlv_parse(uint8_t **data, uint32_t *data_length, uint32_t *type, uint32_t *length, uint8_t **value)
{
        int res = 0;

        assert(data != NULL);
        assert(data_length != NULL);
        assert(type != NULL);
        assert(length != NULL);
        assert(value != NULL);

        if (*data_length < 1)
                return TLV_RESULT_ERROR_NO_HEADER;

        uint8_t header_type = (*data)[0] & 0x0F;
        uint8_t header_value = ((*data)[0] >> 4) & 0x0F;

        *data = *data + 1;
        *data_length = *data_length - 1;

        res = parse_header(type, data, data_length, header_type);
        if (res != TLV_RESULT_SUCCESS)
                return res;
        
        res = parse_header(length, data, data_length, header_value);
        if (res != TLV_RESULT_SUCCESS)
                return res;
        
        if (*data_length < *length)
                return TLV_RESULT_ERROR_INCOMPLETED_FRAME;

        *value = *data;
        *data = *data + *length;
        *data_length = *data_length - *length;

        return TLV_RESULT_SUCCESS;
}

static void put_data_byte(uint8_t **data, uint32_t *data_length, uint8_t data_byte)
{
        **data = data_byte;
        *data = *data + 1;
        *data_length = *data_length - 1;
}

int tlv_format(uint8_t **data, uint32_t *data_length, uint32_t type, uint32_t length, uint8_t *value)
{
        assert(data != NULL);
        assert(data_length != NULL);
        
        uint8_t header = 0;

        if (type < TLV_HEADER_VALUE_1) {
                header |= type & 0x0F;

                if (length < TLV_HEADER_VALUE_1) {
                        header |= (length << 4) & 0xF0;

                        if (*data_length < 1)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);

                } else if (length < (0x0100 + TLV_HEADER_VALUE_1)) {
                        header |= (TLV_HEADER_VALUE_1 << 4) & 0xF0;

                        if (*data_length < 2)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        put_data_byte(data, data_length, length - TLV_HEADER_VALUE_1);

                } else {
                        header |= (TLV_HEADER_VALUE_2 << 4) & 0xF0;

                        if (*data_length < 3)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);

                        uint32_t len = length - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (len >> 8) & 0xFF);
                        put_data_byte(data, data_length, len & 0xFF);

                }
        } else if (type < (0x0100 + TLV_HEADER_VALUE_1)) {
                header |= TLV_HEADER_VALUE_1 & 0x0F;

                if (length < TLV_HEADER_VALUE_1) {
                        header |= (length << 4) & 0xF0;

                        if (*data_length < 2)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        put_data_byte(data, data_length, type - TLV_HEADER_VALUE_1);

                } else if (length < (0x0100 + TLV_HEADER_VALUE_1)) {
                        header |= (TLV_HEADER_VALUE_1 << 4) & 0xF0;

                        if (*data_length < 3)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        put_data_byte(data, data_length, type - TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, length - TLV_HEADER_VALUE_1);

                } else {
                        header |= (TLV_HEADER_VALUE_2 << 4) & 0xF0;

                        if (*data_length < 4)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        put_data_byte(data, data_length, type - TLV_HEADER_VALUE_1);
                        
                        uint32_t len = length - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (len >> 8) & 0xFF);
                        put_data_byte(data, data_length, len & 0xFF);

                }
        } else {
                header |= TLV_HEADER_VALUE_2 & 0x0F;

                if (length < TLV_HEADER_VALUE_1) {
                        header |= (length << 4) & 0xF0;

                        if (*data_length < 3)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);

                        uint32_t tp = type - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (tp >> 8) & 0xFF);
                        put_data_byte(data, data_length, tp & 0xFF);

                } else if (length < (0x0100 + TLV_HEADER_VALUE_1)) {
                        header |= (TLV_HEADER_VALUE_1 << 4) & 0xF0;

                        if (*data_length < 4)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        
                        uint32_t tp = type - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (tp >> 8) & 0xFF);
                        put_data_byte(data, data_length, tp & 0xFF);

                        put_data_byte(data, data_length, length - TLV_HEADER_VALUE_1);

                } else {
                        header |= (TLV_HEADER_VALUE_2 << 4) & 0xF0;

                        if (*data_length < 5)
                                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

                        put_data_byte(data, data_length, header);
                        
                        uint32_t tp = type - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (tp >> 8) & 0xFF);
                        put_data_byte(data, data_length, tp & 0xFF);
                        
                        uint32_t len = length - (0x0100 + TLV_HEADER_VALUE_1);
                        put_data_byte(data, data_length, (len >> 8) & 0xFF);
                        put_data_byte(data, data_length, len & 0xFF);
                }
        }

        if (*data_length < length)
                return TLV_RESULT_ERROR_BUFFER_TOO_SMALL;

        for (uint32_t i = 0; i < length; i++) 
                put_data_byte(data, data_length, value[i]);

        return TLV_RESULT_SUCCESS;
}
