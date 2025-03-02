/**
 * @file protobuf.c
 * @brief Implementation of Protocol Buffer parsing and encoding functions
 */

#include "../include/protobuf.h"
#include <string.h>

bool pb_parse_varint(const uint8_t *data, size_t size, uint64_t *value, size_t *bytes_read) {
    if (!data || !value || !bytes_read || size == 0) {
        return false;
    }

    *value = 0;
    *bytes_read = 0;

    // A varint can be at most 10 bytes for a 64-bit integer
    const size_t max_varint_bytes = 10;
    size_t i;

    for (i = 0; i < size && i < max_varint_bytes; i++) {
        //Get the current byte
        uint8_t byte = data[i];

        // The 7 least significant bits contain the actual data
        // Shift them to the appropriate position and add to the result
        *value |= ((uint64_t)(byte & 0x7F) << (i * 7));

        // If the most significant bit is not set, this is the last byte
        if (!(byte & 0x80)) {
            *bytes_read = i + 1;
            return true;
        }
    }

    // If we've read max_varint_bytes and still haven't found the end, it's an error
    if (i == max_varint_bytes) {
        return false;
    }

    // If we've reached the end of the buffer without finding the end of the varint, it's an error
    return false;
}

bool pb_write_varint(uint64_t value, uint8_t *buffer, size_t size, size_t *bytes_written) {
    if (!buffer || !bytes_written || size == 0) {
        return false;
    }

    *bytes_written = 0;
    
    do {
        // Check if we have enough space in the buffer
        if (*bytes_written >= size) {
            return false;
        }

        // Get the least significant 7 bits
        uint8_t byte = value & 0x7F;

        // Shift the value right by 7 bits
        value >>= 7;
    }
}