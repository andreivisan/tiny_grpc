/**
 * @file protobuf.h
 * @brief Protocol Buffer parsing and encoding functions
 *
 * This file contains functions for parsing and encoding Protocol Buffer
 * data types, including varints and length-delimited strings.
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Parse a varint from a byte stream
 *
 * @param data Pointer to the byte stream
 * @param size Size of the byte stream
 * @param value Pointer to store the parsed value
 * @param bytes_read Pointer to store the number of bytes read
 * @return true if parsing was successful, false otherwise
 */
 bool pb_parse_varint(const uint8_t *data, size_t size, uint64_t *value, size_t *bytes_read);

/**
 * @brief Write a varint to a byte stream
 *
 * @param value The value to encode
 * @param buffer Pointer to the buffer to write to
 * @param size Size of the buffer
 * @param bytes_written Pointer to store the number of bytes written
 * @return true if writing was successful, false otherwise
 */
bool pb_write_varint(uint64_t value, uint8_t *buffer, size_t size, size_t *bytes_written);


/**
 * @brief Parse a length-delimited string from a byte stream
 *
 * @param data Pointer to the byte stream
 * @param size Size of the byte stream
 * @param str Pointer to store the parsed string
 * @param str_size Size of the str buffer
 * @param bytes_read Pointer to store the number of bytes read
 * @return true if parsing was successful, false otherwise
 */
bool pb_parse_string(const uint8_t *data, size_t size, char *str, size_t str_size, size_t *bytes_read);

/**
 * @brief Write a length-delimited string to a byte stream
 *
 * @param str The string to encode
 * @param str_len Length of the string
 * @param buffer Pointer to the buffer to write to
 * @param size Size of the buffer
 * @param bytes_written Pointer to store the number of bytes written
 * @return true if writing was successful, false otherwise
 */
bool pb_write_string(const char *str, size_t str_len, uint8_t *buffer, size_t size, size_t *bytes_written);

/**
 * @brief Get the field number and wire type from a tag
 *
 * @param tag The tag value
 * @param field_number Pointer to store the field number
 * @param wire_type Pointer to store the wire type
 */
void pb_decode_tag(uint64_t tag, uint32_t *field_number, uint8_t *wire_type);

/**
 * @brief Create a tag from a field number and wire type
 *
 * @param field_number The field number
 * @param wire_type The wire type
 * @return The encoded tag
 */
uint64_t pb_encode_tag(uint32_t field_number, uint8_t wire_type);

/**
 * @brief Wire type constants
 */
enum {
    PB_WIRE_TYPE_VARINT = 0,
    PB_WIRE_TYPE_64BIT = 1,
    PB_WIRE_TYPE_LENGTH_DELIMITED = 2,
    PB_WIRE_TYPE_START_GROUP = 3,  // Deprecated
    PB_WIRE_TYPE_END_GROUP = 4,    // Deprecated
    PB_WIRE_TYPE_32BIT = 5
};