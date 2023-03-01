#include "tlv.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
        (void)argc;
        (void)argv;

        const uint32_t chunk_size = 2;
        uint32_t buffer_size = chunk_size;

        uint8_t *buffer = (uint8_t*)malloc(buffer_size);
        uint32_t data_size = 0;
        size_t num;
        
        FILE *input_stream = freopen(NULL, "rb", stdin);

        do {
                num = fread(&buffer[data_size], 1, buffer_size - data_size, input_stream);
                data_size += num;

                if (data_size == buffer_size) {
                        buffer_size += chunk_size;
                        buffer = realloc(buffer, buffer_size);
                }
        } while (num > 0);

        uint8_t *input = buffer;
        uint32_t to_parse = data_size;
        
        int res;
        do {
                uint32_t type;
                uint32_t length;
                uint8_t *value;

                res = tlv_parse(&input, &to_parse, &type, &length, &value);
                if (res == TLV_RESULT_SUCCESS) {
                        char val[length + 1];

                        memcpy((uint8_t*)val, value, length);
                        val[length] = 0;

                        printf("%05d: \"%s\"\n", type, val);
                }
        } while (res == TLV_RESULT_SUCCESS);

        free(buffer);

        return 0;
}
