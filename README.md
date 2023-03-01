![unit-tests](https://github.com/marcinbor85/microtlv/actions/workflows/unit-tests.yml/badge.svg)
# microtlv
Plain C library for parsing and formatting TLV frames.

## Specification
- No dynamic memory allocation
- Full static and state-less operation
- RFC7252 compatible
- Optimised dynamic header length
- Support 0-2 bytes long types
- Support 0-2 bytes long length values
- Platform agnostic (works well on any embedded environment)
- No external dependencies (just standard C library)


## Format
Format is 100% compatible with RFC7252 Coap Options.

```
   0   1   2   3   4   5   6   7
   +---------------+---------------+
   |               |               |
   |      Type     |    Length     |   1 byte
   |               |               |
   +---------------+---------------+
   \                               \
   /             Type              /   0-2 bytes
   \          (extended)           \
   +-------------------------------+
   \                               \
   /            Length             /   0-2 bytes
   \          (extended)           \
   +-------------------------------+
   \                               \
   /                               /
   \                               \
   /             Value             /   0 or more bytes
   \                               \
   /                               /
   \                               \
   +-------------------------------+
```

The fields in an option are defined as follows:

**Type**:  4-bit unsigned integer.  A value between 0 and 12 indicates the **Type**. Three values are reserved for special constructs:

- 13:  An 8-bit unsigned integer follows the initial byte and indicates the **Type** minus 13.
- 14:  A 16-bit unsigned integer in network byte order follows the initial byte and indicates the **Type** minus 269.
- 15:  Reserved


**Length**:  4-bit unsigned integer.  A value between 0 and 12 indicates the length of the **Value**, in bytes. Three values
are reserved for special constructs:

- 13:  An 8-bit unsigned integer precedes the **Value** and indicates the **Length** minus 13.
- 14:  A 16-bit unsigned integer in network byte order precedes the **Value** and indicates the **Length** minus 269.
- 15:  Reserved

**Value**:  A sequence of exactly **Length** bytes.


## Build

Build and test:

```sh
cmake -Bbuild .
cd build
make
make test
```

## API

```c
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
```

## Example tools

Format command line arguments to TLV format:
```sh
./format 1 John 2 Smith 100 "Very Long Text" | xxd
```
Output:
```
00000000: 414a 6f68 6e52 536d 6974 68dd 5701 5665  AJohnRSmith.W.Ve
00000010: 7279 204c 6f6e 6720 5465 7874            ry Long Text
```

Parse input TLV encoded stream:
```sh
./format 1 John 2 Smith 100 "Very Long Text" | ./bin/parse
```
Output:
```
00001: "John"
00002: "Smith"
00100: "Very Long Text"
```
