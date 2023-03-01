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

#ifndef TLV_H
#define TLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define TLV_RESULT_SUCCESS                     0
#define TLV_RESULT_ERROR_NO_HEADER             -1
#define TLV_RESULT_ERROR_INCOMPLETED_FRAME     -2
#define TLV_RESULT_ERROR_BUFFER_TOO_SMALL      -3
#define TLV_RESULT_ERROR_RESERVED_VALUE        -4

#define TLV_HEADER_VALUE_1                      0x0D
#define TLV_HEADER_VALUE_2                      0x0E

/**
 * @brief Parse input buffer data for search TLV-encoded records.
 * 
 * @param data - pointer to input/output buffer pointer (this pointer will be changed during parsing)
 * @param data_length - pointer to input/output data length (should indicate how many bytes left in the buffer)
 * @param type - pointer to record type which will be found
 * @param length - pointer to record value length which will be found
 * @param value - pointer to record value pointer (pointer will be updated without copy data)
 * 
 * @return integer value result, see TLV_RESULT_ macros
 */
int tlv_parse(uint8_t **data, uint32_t *data_length, uint32_t *type, uint32_t *length, uint8_t **value);

/**
 * @brief Format output buffer data with TLV-encoded records.
 * 
 * @param data - pointer to input/output buffer pointer (this pointer will be changed during parsing)
 * @param data_length - pointer to input/output data length (should indicate how many bytes left in the buffer)
 * @param type - record type to encode
 * @param length - record value length to encode
 * @param value - pointer to record value data to encode
 * 
 * @return integer value result, see TLV_RESULT_ macros
 */
int tlv_format(uint8_t **data, uint32_t *data_length, uint32_t type, uint32_t length, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif /* TLV_H */
