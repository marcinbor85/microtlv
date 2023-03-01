# microtlv
Plain C library for parsing and formatting TLV frames.


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
