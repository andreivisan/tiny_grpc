# Tiny gRPC Framework (C Implementation)

A minimal gRPC implementation in C focusing on HTTP/2 protocol details and Protocol Buffer fundamentals.  

## 1. High-Level Framework Features

### Core Components

1. Protocol Buffer Serialization/Deserialization
2. HTTP/2 Protocol Implementation
3. RPC Method Handling
4. Client-Server Communication Stack

### Ancillary Systems

1. Connection Management
2. Error Handling
3. Basic Flow Control
4. Frame Debugging

## Learning

### Protocol Buffer Wire Format: Varint Encoding

**Overview**

Protocol Buffers use a binary serialization format known as the "wire format." One of the key components of this format is varint encoding, which efficiently represents integers using a variable number of bytes.

**Varint Encoding Algorithm**

Varints are a method of serializing integers using one or more bytes. Smaller numbers take fewer bytes, making this encoding efficient for values that are typically small.

**How Varints Work**

1. Each byte in a varint uses:

    - 7 bits for the actual data (the least significant bits)
    - 1 bit (the most significant bit or MSB) as a continuation flag 

2. If the MSB is set (1), it means "more bytes follow"

3. If the MSB is not set (0), this is the last byte of the varint

**Decoding Process**

To decode a varint:

1. Read a byte
2. Extract the 7 least significant bits (LSBs)
3. If the most significant bit (MSB) is 0:
   - This is the last byte
   - Shift the 7 bits by (7 * position) and OR with the result
   - Return the result
4. If the MSB is 1:
   - Shift the 7 bits by (7 * position) and OR with the result
   - Increment position
   - Read the next byte and go to step 2
 
**Encoding Process**

To encode a varint:

1. Take the 7 least significant bits of the number
2. If the remaining value is 0:
   - Set the MSB to 0 (indicating last byte)
   - Write the byte and finish
3. If the remaining value is not 0:
   - Set the MSB to 1 (indicating more bytes follow)
   - Write the byte
   - Shift the value right by 7 bits
   - Go to step 1

**Example**

Let's decode the bytes 0x96 0x01, which should represent the value 150:

```text
Byte 1: 10010110
        │└──┬──┘
        │   └── 7 LSBs = 0010110 (22 decimal)
        └── MSB = 1 (continue to next byte)

Byte 2: 00000001
        │└──┬──┘
        │   └── 7 LSBs = 0000001 (1 decimal)
        └── MSB = 0 (last byte)

Result: 0000001 0010110 = 10010110 binary = 150 decimal
        └─┬─┘ └──┬──┘
          │      └── From first byte (position 0)
          └── From second byte (shifted left by 7 bits)
```

### Protocol Buffer Wire Format: String Encoding

**Overview**

In Protocol Buffers, strings are encoded using a wire type called "length-delimited" (wire type 2). This encoding is used not only for strings but also for bytes, embedded messages, and packed repeated fields.

**String Encoding Algorithm**

Strings in Protocol Buffers are encoded as a sequence of bytes with a length prefix. The length prefix itself is encoded as a varint.

**Encoding Process**

To encode a string:

1. Calculate the length of the string in bytes
2. Encode the length as a varint
3. Append the raw string bytes after the length

**Decoding Process**

To decode a string:

1. Read and decode a varint to get the length
2. Read exactly that many bytes from the stream
3. Interpret those bytes as a string (typically UTF-8 encoded)

**Example**

Let's encode the string "hello" in Protocol Buffers:

1. The string "hello" is 5 bytes long
2. Encode 5 as a varint: 0x05 (since it's a small number, it fits in one byte)
3. The UTF-8 encoding of "hello" is: 68 65 6c 6c 6f (in hex)
4. Combine: 05 68 65 6c 6c 6f

So the complete encoding is:

```text
05 68 65 6c 6c 6f
│  └───┬────┘
│      └── Raw string bytes ("hello")
└── Length prefix (5 bytes, encoded as a varint)
```

**Field Encoding**

When a string is encoded as a field in a message, it includes a field tag that combines the field number and wire type:

1. Calculate the field tag: (field_number << 3) | wire_type
   - For strings, wire_type = 2 (length-delimited)
2. Encode the field tag as a varint
3. Encode the string as described above

For example, if "hello" is field number 1:

1. Field tag = (1 << 3) | 2 = 8 | 2 = 10 (decimal) = 0x0A (hex)
2. Encode field tag as varint: 0x0A
3. Encode string: 05 68 65 6c 6c 6f
4. Complete field: 0A 05 68 65 6c 6c 6f

**Usage in Protocol Buffers**

String encoding is used for:

   - string fields (UTF-8 encoded text)
   - bytes fields (arbitrary byte sequences)
   - Embedded message fields (nested Protocol Buffer messages)
   - Packed repeated fields (multiple values of the same type stored contiguously)

Understanding string encoding is essential for working with text data, binary data, and complex nested structures in Protocol Buffers and gRPC.