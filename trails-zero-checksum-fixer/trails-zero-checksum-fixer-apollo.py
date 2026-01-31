def loh_trails_checksum(save_data) -> Tuple[int, int]:
    """Returns the custom double checksum for LoH Trails Zero/Azure games"""
    # Get file size
    file_size = len(save_data)
    
    # Ensure file is large enough for the calculation
    if file_size < 0x08:
        return None
        
    file_size_max_pos = file_size - 1
    
    # Use integer division for the calculation
    file_size_checksum = -1 * (((file_size_max_pos - 0x08) // 0x04) + 0x01)
    file_size_checksum &= 0xFFFFFFFF  # Keep as 32-bit
    
    # Process the file, excluding the last 8 bytes
    # dwadd result is in bytes (big-endian format)
    dwadd = uhashlib.dwadd_le(save_data[:(file_size_max_pos - 0x08)])
    file_savedata_checksum = struct.unpack('>I', dwadd)[0]
    
    # Final calculation with 32-bit masking
    file_size_checksum = (file_size_checksum - file_savedata_checksum) & 0xFFFFFFFF
    
    return (file_savedata_checksum, file_size_checksum)
