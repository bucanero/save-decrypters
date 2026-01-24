#!/usr/bin/env python3
"""
Max Payne 3 PS3 Save Decrypter - (c) 2025 by Bucanero
Converted to Python from C original
https://github.com/bucanero/save-decrypters/tree/master/maxpayne3-decrypter

This tool is based on crypto notes by Vulnavia, and PS3 keys and code by Red-EyeX32
"""

import ustruct as struct
import uhashlib
import ucrypto
import ubinascii
import gc

# Max Payne 3 init key
BASE_PROFILE_KEY = "PS35675hA"
PROFILE_KEY_SIZE = 0x32
MODE_ENCRYPT = True
MODE_DECRYPT = False
MAX_DECOMP_BUF_SIZE = 0x0C800

# Static AES Key for Max Payne 3 Save data encryption
STATIC_AES_KEY = bytes([
    0x1A, 0xB5, 0x6F, 0xED, 0x7E, 0xC3, 0xFF, 0x01, 0x22, 0x7B, 0x69, 0x15, 0x33, 0x97, 0x5D, 0xCE,
    0x47, 0xD7, 0x69, 0x65, 0x3F, 0xF7, 0x75, 0x42, 0x6A, 0x96, 0xCD, 0x6D, 0x53, 0x07, 0x56, 0x5D
])

PKDF_SALT1 = bytes([0x0F, 0xC9, 0x19, 0xE8, 0x9A, 0x17, 0xC4, 0x5F, 0xE7, 0x16, 0xD4, 0x6C, 0x3A, 0x15, 0x9C, 0x75])
PKDF_SALT2 = bytes([0xE1, 0x09, 0xA5, 0x42, 0xF6, 0x0A, 0x13, 0x3B, 0x81, 0xAC, 0x02, 0x55, 0xCC, 0x39, 0x40, 0x1B])
PKDF_SALT3 = bytes([0x15, 0x08, 0xE9, 0x6F, 0x47, 0xB8, 0x47, 0xD1, 0x3A, 0x65, 0x8C, 0x71, 0x00, 0x00, 0x00, 0x00])


def bytes_to_hex(data: bytes) -> str:
    """Convert bytes to hex string."""
    return ubinascii.hexlify(data)


def pbkdf2_sha1(password: bytes, salt:  bytes, iterations: int, dklen: int) -> bytes:
    """Derive a key using PBKDF2 with SHA1."""
    return uhashlib.pbkdf2_sha1(password, salt, iterations, dklen)


def hmac_sha1(key: bytes, data: bytes) -> bytes:
    """HMAC-SHA1 implementation"""
    return uhashlib.hmac_sha1(key, data)


def sha1_hash(data: bytes) -> bytes:
    """Calculate SHA1 hash."""
    return uhashlib.sha1(data)


def mp3_aes_ecb(key: bytes, data: bytearray, encrypt: bool) -> None:
    """MP3 AES ECB encryption/decryption."""
    data_len = len(data) & ~0x0F
    
    if data_len > 0:
        for i in range(16):
            if encrypt:
                for j in range(0, data_len, 16):
                    encrypted = data[j:j+16]
                    ucrypto.aes_ecb(ucrypto.ENCRYPT, encrypted, key)
                    data[j:j+16] = encrypted
            else:
                for j in range(0, data_len, 16):
                    decrypted = data[j:j+16]
                    ucrypto.aes_ecb(ucrypto.DECRYPT, decrypted, key)
                    data[j:j+16] = decrypted


def mp3_aes_ecb2(key: bytes, data: bytearray, encrypt: bool) -> None:
    """MP3 AES ECB encryption/decryption (last block only)."""
    data_len = len(data) & ~0x0F
    
    if data_len > 0:
        for i in range(16):
            if encrypt: 
                encrypted = data[-16:]
                ucrypto.aes_ecb(ucrypto.ENCRYPT, encrypted, key)
                data[-16:] = encrypted
            else: 
                decrypted = data[-16:]
                ucrypto.aes_ecb(ucrypto.DECRYPT, decrypted, key)
                data[-16:] = decrypted


def decrypt_data(save_buffer: bytearray, profile_key: str) -> None:
    """Decrypt Max Payne 3 save data."""
    save_len = len(save_buffer) - 0x2C
    profile_key_bytes = profile_key.encode('ascii')
    
    print("[*] Total Decrypted Size Is 0x{:X} ({:d} bytes)".format(save_len, save_len))
    
    # Prepare header data and save data
    header_data = bytearray(save_buffer[:44])
    save_data = bytearray(save_buffer[0x2C:])
    
    # Add big-endian save length
    be_save_data_len = struct.pack('>I', save_len)
    
    # Calculate HMAC-SHA1
    hmac_data = save_data + be_save_data_len
    digest_key = hmac_sha1(profile_key_bytes, hmac_data)
    
    print("[*] HMAC-SHA1:", bytes_to_hex(digest_key))
    
    # Derive key1
    pkdf2_key1 = pbkdf2_sha1(digest_key, PKDF_SALT1, 2000, 0x20)

    # Decrypt header
    mp3_aes_ecb(pkdf2_key1, header_data, MODE_DECRYPT)

    # Derive key2
    pkdf2_key2 = pbkdf2_sha1(header_data, PKDF_SALT2, 2000, 0x20)
    pkdf2_key2_copy = bytearray(pkdf2_key2)
    mp3_aes_ecb(STATIC_AES_KEY, pkdf2_key2_copy, MODE_ENCRYPT)
    pkdf2_key2 = bytes(pkdf2_key2_copy)
    
    # Decrypt save data
    mp3_aes_ecb2(pkdf2_key2, save_data, MODE_DECRYPT)
    mp3_aes_ecb(pkdf2_key2, save_data, MODE_DECRYPT)
    
    # Verify the data
    tmpbuf = bytearray(PROFILE_KEY_SIZE + 4)
    tmpbuf[:len(profile_key_bytes)] = profile_key_bytes
    tmpbuf[0x32:0x36] = be_save_data_len
    sha1key = sha1_hash(tmpbuf)
    
    hmac_init_key2 = bytearray(hmac_sha1(sha1key, save_data))
    hmac_init_key2[0x14:0x18] = header_data[0x20:0x24]  # integer_7
    hmac_init_key2[0x18:0x20] = header_data[0x24:0x2C]  # long_1
    
    salt_buffer = bytearray(PKDF_SALT3)
    salt_buffer[0x0C:0x10] = header_data[0x20:0x24]  # integer_7

    # Derive key3
    pkdf2_key3 = pbkdf2_sha1(bytes(hmac_init_key2), salt_buffer, 2000, 0x20)

    # Decrypt header blob_1
    blob_1 = bytearray(header_data[0: 0x20])
    mp3_aes_ecb(STATIC_AES_KEY, blob_1, MODE_DECRYPT)
    mp3_aes_ecb(pkdf2_key3, blob_1, MODE_DECRYPT)
    
    header_data[0:0x20] = blob_1
    
    print("[*] Data Hash:", bytes_to_hex(bytes(blob_1)))
    
    if bytes(blob_1) != bytes(hmac_init_key2):
        print("[!] Warning: save data could not be verified.")
    
    print("[*] Decrypted File Successfully!")
    
    # Copy decrypted header back
    save_buffer[:0x2C] = header_data
    save_buffer[0x2C:] = save_data


def encrypt_data(save_buffer: bytearray, profile_key: str) -> None:
    """Encrypt Max Payne 3 save data."""
    save_len = len(save_buffer) - 0x2C
    profile_key_bytes = profile_key.encode('ascii')
    
    print("[*] Total Encrypted Size Is 0x{:X} ({} bytes)".format(save_len, save_len))
    
    header_data = bytearray(save_buffer[:0x2C])
    save_data = bytearray(save_buffer[0x2C:0x2C + save_len])
    
    pb_save_data_len = struct.pack('>I', save_len)
    
    # Calculate HMAC-SHA1
    tmpbuf = bytearray(PROFILE_KEY_SIZE + 4)
    tmpbuf[:len(profile_key_bytes)] = profile_key_bytes
    tmpbuf[0x32:0x36] = pb_save_data_len
    sha1key = sha1_hash(tmpbuf)
    
    hmac_init_key2 = bytearray(hmac_sha1(sha1key, save_data))
    hmac_init_key2[0x14:0x18] = header_data[0x20:0x24]  # integer_7
    hmac_init_key2[0x18:0x20] = header_data[0x24:0x2C]  # long_1
    
    salt_buffer = bytearray(PKDF_SALT3)
    salt_buffer[0x0C:0x10] = header_data[0x20:0x24]  # integer_7
    
    # Derive key3
    pkdf2_key3 = pbkdf2_sha1(bytes(hmac_init_key2), salt_buffer, 2000, 0x20)
    
    # Store new header blob_1
    print("[*] Old Hash :", bytes_to_hex(header_data[: 0x20]))
    print("[*] New Hash :", bytes_to_hex(bytes(hmac_init_key2)))
    
    header_data[: 0x20] = hmac_init_key2
    
    # Encrypt header blob_1
    blob_1 = bytearray(header_data[: 0x20])
    mp3_aes_ecb(pkdf2_key3, blob_1, MODE_ENCRYPT)
    mp3_aes_ecb(STATIC_AES_KEY, blob_1, MODE_ENCRYPT)
    header_data[:0x20] = blob_1
    
    # Derive key2
    pkdf2_key2 = pbkdf2_sha1(header_data, PKDF_SALT2, 2000, 0x20)
    pkdf2_key2_copy = bytearray(pkdf2_key2)
    mp3_aes_ecb(STATIC_AES_KEY, pkdf2_key2_copy, MODE_ENCRYPT)
    pkdf2_key2 = bytes(pkdf2_key2_copy)
    
    # Encrypt save data
    mp3_aes_ecb(pkdf2_key2, save_data, MODE_ENCRYPT)
    mp3_aes_ecb2(pkdf2_key2, save_data, MODE_ENCRYPT)
    
    # Calculate HMAC-SHA1
    hmac_data = save_data + pb_save_data_len
    digest_key = hmac_sha1(profile_key_bytes, hmac_data)
    print("[*] HMAC-SHA1:", bytes_to_hex(digest_key))
    
    # Derive key1
    pkdf2_key1 = pbkdf2_sha1(digest_key, PKDF_SALT1, 2000, 0x20)
    
    # Encrypt header
    mp3_aes_ecb(pkdf2_key1, header_data, MODE_ENCRYPT)
    
    save_buffer[:0x2C] = header_data
    save_buffer[0x2C:0x2C + save_len] = save_data


def lz77_decompress_data(data: bytes) -> Optional[bytes]:
    """Decompress LZ77 compressed data."""
    if len(data) < 2:
        print("[!] Decompression error: invalid input size.")
        return None
    
    declared_size = data[0] + (data[1] << 8) + 2
    if declared_size != len(data):
        print("[!] Decompression error:  invalid input size.")
        return None
    
    outstream = bytearray(MAX_DECOMP_BUF_SIZE)
    pos = 2
    op_pos = 0
    
    while pos < len(data):
        code = data[pos]
        pos += 1
        
        for i in range(8):
            if pos >= len(data):
                break
            
            code &= 0xFF
            
            if code & 0x80:
                if op_pos < MAX_DECOMP_BUF_SIZE: 
                    outstream[op_pos] = data[pos]
                    op_pos += 1
                pos += 1
            else: 
                if pos + 1 >= len(data):
                    break
                
                k = data[pos]
                pos += 1
                
                off = op_pos - 1
                off -= (k >> 4) | (data[pos] << 4)
                pos += 1
                
                if off < 0:
                    print("[!] Decompression error: invalid offset.")
                    return None
                
                length = (k & 0x0F) + 3
                for j in range(length):
                    if op_pos >= MAX_DECOMP_BUF_SIZE: 
                        break
                    outstream[op_pos] = outstream[off]
                    op_pos += 1
                    off += 1
            
            code <<= 1
    
    return bytes(outstream[: op_pos])


def lz77_compress_data(data: bytes) -> Optional[bytes]:
    """Compress data using LZ77 compression."""
    in_size = len(data)
    max_out_size = in_size + (in_size // 8) + 16
    
    outstream = bytearray(max_out_size)
    op_pos = 2  # Reserve space for size header
    ip_pos = 0
    
    while ip_pos < in_size:
        code = 0
        code_pos = op_pos
        op_pos += 1
        
        for i in range(8):
            if ip_pos >= in_size:
                break
            
            best_len = 0
            best_off = 0
            
            # Search window:  up to 4096 bytes back
            search_start = max(0, ip_pos - 4096)
            
            for j in range(search_start, ip_pos):
                match_len = 0
                
                # Count matching bytes (max 18)
                while (match_len < 18 and 
                       ip_pos + match_len < in_size and 
                       data[j + match_len] == data[ip_pos + match_len]):
                    match_len += 1
                
                # Only consider matches of length 3 or more
                if match_len >= 3 and match_len > best_len:
                    best_len = match_len
                    best_off = ip_pos - j
            
            if best_len >= 3 and best_len <= 18:
                # Encode as back-reference (bit = 0)
                offset = best_off - 1
                length = best_len - 3
                
                outstream[op_pos] = ((offset & 0x0F) << 4) | (length & 0x0F)
                op_pos += 1
                outstream[op_pos] = (offset >> 4) & 0xFF
                op_pos += 1
                
                ip_pos += best_len
            else:
                # Encode as literal (bit = 1)
                code |= (1 << (7 - i))
                outstream[op_pos] = data[ip_pos]
                op_pos += 1
                ip_pos += 1
        
        outstream[code_pos] = code
    
    # Write size header (compressed size - 2)
    compressed_size = op_pos - 2
    outstream[0] = compressed_size & 0xFF
    outstream[1] = (compressed_size >> 8) & 0xFF
    
    return bytes(outstream[: op_pos])


def add_sum(data: bytes) -> int:
    """Calculate checksum (sum of all bytes)."""
    return sum(data) & 0xFFFFFFFF


def maxpayne3_crypt(mode, data):
    """Main function."""
    print("max-payne3-decrypter 0.2.0 - (c) 2026 by Bucanero\n")
    
    # Parse header
    if len(data) < 0x20 or data[: 4] != b'mp3\x00':
        print("[*] Invalid header detected.")
        return -1
    
    header_length = struct.unpack('>I', data[4:8])[0]
    if header_length < 0x1A: 
        print("[*] Invalid header detected.")
        return -1
    
    save_buffer_len = struct.unpack('>I', data[8:12])[0]
    save_data_len = struct.unpack('>I', data[12:16])[0]
    
    if (save_buffer_len + header_length) != save_data_len:
        print("[*] Invalid save size detected.")
        return -1
    
    stored_sum = struct.unpack('>I', data[16:20])[0]
    version = struct.unpack('>I', data[20:24])[0]
    
    print("[*] Stored Checksum: {:08X}".format(stored_sum))
    print("[*] Header Length:", header_length)
    print("[*] Save Buffer Length:", save_buffer_len)
    print("[*] Save Data Length:", save_data_len)
    print("[*] Version:", version)
    
    # Create the save key
    profile_key = "{}{:02d}{}".format(BASE_PROFILE_KEY, header_length, version)[: PROFILE_KEY_SIZE]
    print("[*] Profile Key:", profile_key)
        
    if mode == MODE_DECRYPT:
        # Decrypt mode
        calculated_sum = add_sum(data[0x44:0x44 + save_buffer_len])
        if calculated_sum != stored_sum:
            print("[!] Warning: Invalid checksum detected.")
        
        # Extract and decrypt
        decrypt_buffer = bytearray(data[0x18:0x18 + 0x2C + save_buffer_len])
        decrypt_data(decrypt_buffer, profile_key)
        
        # update header
        data[0x18:0x18 + 0x2C] = decrypt_buffer[:0x2C]

        # Decompress
        decompressed_data = lz77_decompress_data(bytes(decrypt_buffer[0x2C:0x2C + save_buffer_len]))
        if not decompressed_data:
            print("[!] ERROR:  Decompression failed, can't decrypt data")
            return -1
        
        decomp_size = len(decompressed_data)
        print("[*] Compressed Size: {} bytes".format(save_buffer_len))
        print("[*] Decompressed Size: {} bytes\n".format(decomp_size))
        
        # Update data
        data[0x44:0x44 + decomp_size] = decompressed_data
        save_buffer_len = decomp_size
        data[8:12] = struct.pack('>I', save_buffer_len)
        save_data_len = save_buffer_len + header_length
        data[12:16] = struct.pack('>I', save_data_len)
    
    elif mode == MODE_ENCRYPT:
        # Encrypt mode
        compressed_data = lz77_compress_data(bytes(data[0x44:0x44 + save_buffer_len]))
        
        if not compressed_data:
            print("[!] ERROR: Compression failed, can't encrypt data")
            return -1
        
        compressed_size = len(compressed_data)
        print("[*] Compressed Size: {} bytes (from {} bytes)".format(compressed_size, save_buffer_len))
        
        if compressed_size > (len(data) - 0x44):
            print("[!] ERROR: Compressed data is larger than available file buffer")
            return -1
        
        # Update data
        data[0x44:0x44 + compressed_size] = compressed_data
        save_buffer_len = compressed_size
        data[8:12] = struct.pack('>I', save_buffer_len)
        save_data_len = save_buffer_len + header_length
        data[12:16] = struct.pack('>I', save_data_len)
        
        print("[*] New Save Buffer Length: {}".format(save_buffer_len))
        print("[*] New Save Data Length: {}".format(save_data_len))
        
        # Encrypt
        encrypt_buffer = bytearray(data[0x18:0x18 + 0x2C + save_buffer_len])
        encrypt_data(encrypt_buffer, profile_key)
        data[0x18:0x18 + len(encrypt_buffer)] = encrypt_buffer
        
        stored_sum = add_sum(data[0x44:0x44 + save_buffer_len])
        data[16:20] = struct.pack('>I', stored_sum)
        print("[*] New Checksum: {:08X}\n".format(stored_sum))
    
    else:
        raise Exception("Error: Unknown mode '{}', must be 'MODE_ENCRYPT' or 'MODE_DECRYPT'".format(mode))

    # Clean up
    gc.collect()

    return 0
