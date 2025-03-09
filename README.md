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