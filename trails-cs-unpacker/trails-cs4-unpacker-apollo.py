# loh_trails.py: A simple module to generate The Legend of Heroes Trails... CRC32 checksum

import apollo
import uhashlib
import ustruct as struct


class Backref:
    def __init__(self, Position=0, Length=0):
        self.Position = Position
        self.Length = Length

def reverseBits(n: int) -> int:
    """
    Reverse the bits of a 32-bit unsigned integer.
    """
    result = 0
  
    for i in range(32):
        result |= (n & 1) << (31 - i)
        n >>= 1
      
    return result

def loh_trails_crc32(savedata, start_off):
    """Returns the custom CRC32 for LoH Trails games"""
    crc_init = reverseBits(len(savedata) - start_off)
    print("CRC Init: 0x{:08X}".format(crc_init))

    trails_crc = uhashlib.crc(savedata[start_off:], uhashlib.CRC_32_BITS, 0x04C11DB7, crc_init, 0, 1, 1)
    apollo.endian_swap(trails_crc)

    return trails_crc


def decompress_type1(target: bytearray,
                     uncompressed_size: int,
                     compressed_data: bytes,
                     compressed_size: int) -> bool:
    if compressed_size < 12:
        return False

    # decompression algorithm derived from 0x41aa50 in CS2
    # very simple, can only copy verbatim bytes or reference already written bytes
    # Read 32-bit little-endian values from the compressed data
    source_size, backref_byte = struct.unpack_from('<II', compressed_data, 4)
    
    target_position = 0
    source_position = 12
    
    while source_position < source_size:
        if target_position >= uncompressed_size:
            return False
            
        if source_position >= compressed_size:
            return False
            
        source_byte = compressed_data[source_position]
        source_position += 1
        
        if source_byte == backref_byte:
            if source_position >= compressed_size:
                return False
                
            backref_offset = compressed_data[source_position]
            source_position += 1
            
            if backref_offset == backref_byte:
                target[target_position] = backref_byte
                target_position += 1
            else:
                if backref_byte < backref_offset:
                    backref_offset -= 1
                    
                if backref_offset == 0:
                    return False
                    
                if source_position >= compressed_size:
                    return False
                    
                backref_length = compressed_data[source_position]
                source_position += 1
                
                if backref_length > 0:
                    if target_position < backref_offset:
                        return False
                        
                    # Check for overflow (Python handles large ints, but we mimic C behavior)
                    if target_position > 0xFFFFFFFF - backref_length:
                        return False
                        
                    if target_position + backref_length > uncompressed_size:
                        return False
                    
                    # Copy bytes one at a time to handle overlapping ranges correctly
                    for i in range(backref_length):
                        target[target_position + i] = target[(target_position - backref_offset) + i]
                    
                    target_position += backref_length
        else:
            target[target_position] = source_byte
            target_position += 1
    
    if uncompressed_size != target_position:
        return False
    
    return True


# Helper function for easier usage
def loh_trails_decompress(compressed_data: bytes) -> bytearray | None:
    """Wrapper that handles the buffer creation and returns the decompressed data."""
    if len(compressed_data) < 12:
        return None
    
    # Read expected sizes from header
    target_size, source_size = struct.unpack_from('<II', compressed_data, 0)
    
    print("Compressed size: {} bytes".format(source_size))
    print("Decompressed size: {} bytes".format(target_size))

    # Validate sizes
    if source_size != len(compressed_data):
        raise Exception("Compressed size mismatch")
    
    # Create buffer for decompressed data
    uncompressed_data = bytearray(target_size)
    
    # Perform decompression
    success = decompress_type1(
        uncompressed_data,
        target_size,
        compressed_data,
        len(compressed_data)
    )
    
    if success:
        print("Decompression successful: {} bytes".format(len(uncompressed_data)))
    else:
        raise Exception("Decompression failed")

    return uncompressed_data


def compress_type1_calculate_byte_counts(uncompressed_data: bytes):
    """
    Count occurrences of each byte value in the uncompressed data.
    Returns a list of 256 integers.
    """
    count_per_byte = [0] * 256
    for byte in uncompressed_data:
        count_per_byte[byte] += 1
    return count_per_byte

def compress_type1_get_least_used_byte(count_per_byte) -> int:
    """
    Find the byte value that occurs least frequently.
    """
    least_used_byte = 0
    for i in range(1, 256):
        if count_per_byte[i] < count_per_byte[least_used_byte]:
            least_used_byte = i
    return least_used_byte

def compress_type1_bound_for_backref_byte(
    count_per_byte,
    uncompressed_length: int,
    backref_byte: int
) -> int:
    """
    Calculate the maximum bound for compressed data size.
    """
    return 12 + uncompressed_length + count_per_byte[backref_byte]

def find_best_backref(
    uncompressed_data: bytes,
    uncompressed_length: int,
    uncompressed_position: int
) -> Backref:
    """
    Find the best back reference at the current position.
    """
    MAX_BACKREF_LENGTH = 255
    MAX_BACKREF_OFFSET = 254
    
    best_backref = Backref(Position=0, Length=0)
    
    if uncompressed_position == 0:
        return best_backref  # no backref possible
    
    # Calculate search range
    first_possible_backref_position = (
        0 if uncompressed_position < MAX_BACKREF_OFFSET 
        else uncompressed_position - MAX_BACKREF_OFFSET
    )
    last_possible_backref_position = uncompressed_position - 1
    
    current_backref_test = last_possible_backref_position
    
    while True:
        # Count how many bytes we can match from this position
        count = 0
        local_max_backref_length = uncompressed_position - current_backref_test
        allowed_backref_length = min(
            local_max_backref_length,
            uncompressed_length - uncompressed_position
        )
        
        for i in range(allowed_backref_length):
            if (uncompressed_data[current_backref_test + i] == 
                uncompressed_data[uncompressed_position + i]):
                count += 1
            else:
                break
        
        if count > best_backref.Length:
            best_backref = Backref(Position=current_backref_test, Length=count)
        
        if count == MAX_BACKREF_LENGTH:
            break
        
        if current_backref_test == first_possible_backref_position:
            break
        
        current_backref_test -= 1
    
    return best_backref


def compress_type1(max_bound,
    uncompressed_data: bytes,
    backref_byte: int
) -> Tuple[bool, bytes]:
    """
    Compress data using Type1 compression.
    Returns: (success, compressed_data)
    """
    uncompressed_length = len(uncompressed_data)
    
    # Allocate buffer with maximum possible size
    compressed_data = bytearray(max_bound)
    
    output_position = 12
    pos = 0
    prev = -1
    
    while pos < uncompressed_length:
        best_backref = find_best_backref(uncompressed_data, uncompressed_length, pos)
        
        pct = (pos * 100) // uncompressed_length
        if (pct % 10) == 0 and pct != prev:
            print("Compressing... {}%".format(pct))
            prev = pct

        if best_backref.Length <= 1:
            # Could not find a sensible backref, need to write literal
            byte = uncompressed_data[pos]
            if byte == backref_byte:
                compressed_data[output_position] = byte
                compressed_data[output_position + 1] = byte
                output_position += 2
            else:
                compressed_data[output_position] = byte
                output_position += 1
            pos += 1
            continue
        
        if best_backref.Length >= 4:
            compressed_data[output_position] = backref_byte
            offset = pos - best_backref.Position
            # Apply the same adjustment as in C code
            if backref_byte <= offset:
                offset += 1
            compressed_data[output_position + 1] = offset & 0xFF
            compressed_data[output_position + 2] = best_backref.Length & 0xFF
            output_position += 3
            pos += best_backref.Length
            continue
        
        # Backref of 2 or 3 bytes - write as literal (as per original implementation)
        byte = uncompressed_data[pos]
        if byte == backref_byte:
            compressed_data[output_position] = byte
            compressed_data[output_position + 1] = byte
            output_position += 2
        else:
            compressed_data[output_position] = byte
            output_position += 1
        pos += 1
    
    if output_position > 0xFFFFFFFF:  # UINT32_MAX
        return False, b''
    
    compressed_length_local = output_position
    
    # Write header in little endian
    struct.pack_into("<III", compressed_data, 0, uncompressed_length, compressed_length_local, backref_byte)
    
    # Trim to actual size
    return True, bytes(compressed_data[:compressed_length_local])

# Convenience function that does everything
def loh_trails_compress(uncompressed_data: bytes) -> bytes:
    """
    Full compression pipeline: calculates best backref byte and compresses.
    Returns: (compressed_data)
    """
    count_per_byte = compress_type1_calculate_byte_counts(uncompressed_data)
    backref_byte = compress_type1_get_least_used_byte(count_per_byte)
    max_bound = compress_type1_bound_for_backref_byte(count_per_byte, len(uncompressed_data), backref_byte)

    print("Best backref byte: {:02X}".format(backref_byte))
    print("Original size: {} bytes".format(len(uncompressed_data)))

    success, compressed = compress_type1(max_bound, uncompressed_data, backref_byte)
    if success:
        print("Compressing... 100%")
        print("Compressed size: {} bytes".format(len(compressed)))
    else:
        raise Exception("Compression failed")

    return compressed
