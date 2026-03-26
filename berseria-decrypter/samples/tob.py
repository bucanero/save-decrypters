import hashlib
import struct


entries = [
    (0x12e08ee7, 0x7b0,   0x3000, 0x64),
    (0x737d5409, 0x37b0,  0x200,  0x64),
    (0x479878ac, 0x39b0,  0x30000,0x12c),
    (0x46d3d6d1, 0x339b0, 0x800,  0x65),
    (0x85495621, 0x341b0, 0x1000, 0x64),
    (0x8beb4748, 0x351b0, 0x5000, 0x73),
    (0x8beb4749, 0x3a1b0, 0x5000, 0x73),
    (0x8beb474e, 0x3f1b0, 0x5000, 0x73),
    (0x8beb474f, 0x441b0, 0x5000, 0x73),
    (0x8beb474c, 0x491b0, 0x5000, 0x73),
    (0x8beb474d, 0x4e1b0, 0x5000, 0x73),
    (0x3476c027, 0x531b0, 0x400,  0x64),
    (0x1eceb884, 0x535b0, 0x2000, 0x64),
    (0x1e76c617, 0x555b0, 0x3800, 0x64),
    (0x369c0439, 0x58db0, 0x800,  0x64),
    (0x80e6d14d, 0x595b0, 0x1800, 0x64),
    (0xdd719d8a, 0x5adb0, 0x800,  0x64),
    (0x582851f8, 0x5b5b0, 0x800,  0x64),
    (0x4adbef07, 0x5bdb0, 0x104,  0x100),
    (0xbd58502a, 0x5bec0, 0xbc,   0x100),
    (0x73d39d7b, 0x5bf80, 0x54,   0x100),
    (0x7e2f7d56, 0x5bfe0, 0x58,   0x100),
    (0x53ca8105, 0x5c040, 0x90,   0x100),
    (0x8d845c13, 0x5c0d0, 0x1140, 0x100),
    (0x30439cbc, 0x5d210, 0x111bc,0x100),
    (0xfc196efb, 0x6e3d0, 0x24c,  0x100),
    (0x44354cb1, 0x6e620, 0x1a0,  0x100),
    (0x4507e93a, 0x6e7c0, 0x140,  0x100),
    (0xf20ba24d, 0x6e900, 0x1d0,  0x100),
    (0x2627cc47, 0x6ead0, 0xe0,   0x100),
    (0xcfb6927b, 0x6ebb0, 0x170,  0x100),
    (0x246b9df9, 0x6ed20, 0x1a0,  0x100),
    (0x31b5b571, 0x6eec0, 0x260,  0x100),
    (0x7ba6b5ef, 0x6f120, 0xbc,   0x100),
    (0x6b3df98c, 0x6f1e0, 0x6c,   0x100),
    (0xb6557435, 0x6f250, 0x84,   0x100),
    (0x84ddff98, 0x6f2e0, 0xcc,   0x100),
    (0x7a56f4e7, 0x6f3b0, 0xe0,   0x100),
    (0xb8e76c4e, 0x6f490, 0x50,   0x100),
    (0xdb8b1981, 0x6f4e0, 0x50,   0x100),
]


def sha1_custom(data, key):
    h = hashlib.sha1()
    h.update(data)
    h.update(key)
    return h.digest()

def align(offset, boundary=0x10):
    remainder = offset % boundary
    if remainder == 0:
        return offset
    return offset + (boundary - remainder)

def decrypt_entry(buf, entry_offset):
    item_id     = int.from_bytes(buf[entry_offset+0x00:entry_offset+0x04], 'little')
    data_offset = int.from_bytes(buf[entry_offset+0x04:entry_offset+0x08], 'little')
    data_len    = int.from_bytes(buf[entry_offset+0x08:entry_offset+0x0C], 'little')
    flags       = int.from_bytes(buf[entry_offset+0x0C:entry_offset+0x10], 'little')
    nonce       = bytes(buf[entry_offset+0x10:entry_offset+0x1C])  # 12 bytes
    sha1_key    = bytes(buf[entry_offset+0x1C:entry_offset+0x30])  # 20 bytes = XOR keystream

    enc_data = bytearray(buf[data_offset:data_offset+data_len])

    plaintext = bytearray()
    for i in range(data_len):
        dec = enc_data[i] ^ (i & 0xFF) ^ sha1_key[i % 0x14]
        plaintext.append(dec)

    # SHA1 check: sha1(plaintext + nonce)
    computed = sha1_custom(bytes(plaintext), nonce)
    if computed == sha1_key:
        verified = "OK"
    else:
        verified = f"FAIL (expected={sha1_key.hex()} got={computed.hex()})"

    return item_id, data_offset, data_len, flags, plaintext, verified


def decrypt_save(filepath):
    with open(filepath, 'rb') as f:
        buf = bytearray(f.read())

    magic       = int.from_bytes(buf[0x00:0x04], 'little')
    entry_count = int.from_bytes(buf[0x04:0x08], 'little')
    data_size   = int.from_bytes(buf[0x08:0x0C], 'little')

    print(f"Magic:       {magic}")
    print(f"Entries:     {entry_count}")
    print(f"Data size:   {hex(data_size)}")

    out = bytearray()
    all_ok = True

    
    for i in range(entry_count):
        entry_offset = 0x30 + i * 0x30
        item_id, data_offset, data_len, flags, plaintext, verified = decrypt_entry(buf, entry_offset)
        
        
        padded_len = align(data_len)
        out += plaintext
        out += bytearray(padded_len - data_len)

    if all_ok:
        print("All entries verified OK!")
    else:
        print("WARNING: some entries failed SHA1 check")

    with open(filepath+ '.dec', 'wb')as f:
        f.write(out)

    return 


def patch_save(data_buffer):
    chunk1 = data_buffer[0x00:0x701e0]
    chunk2 = bytearray.fromhex('11' * 16)

    sha1 = hashlib.sha1()
    sha1.update(chunk1)
    sha1.update(chunk2)
    new_hash = sha1.digest()

    print(f"Calculated Hash: {new_hash.hex().upper()}")
    return new_hash


def encrypt_entry(plaintext):

    nonce = bytes.fromhex('11' *12)
    
    # Compute SHA1 of plaintext + nonce → this becomes the XOR keystream
    sha1_key = sha1_custom(bytes(plaintext), nonce)
    
    # XOR encrypt
    enc_data = bytearray()
    for i in range(len(plaintext)):
        enc = plaintext[i] ^ (i & 0xFF) ^ sha1_key[i % 0x14]
        enc_data.append(enc)
    
    return enc_data, sha1_key, nonce

def encrypt_save(file_path):
    with open(file_path, 'rb') as f:
        data = bytearray(f.read())

    # Rebuild encrypted data region (same size as original 0x701e0)
    data_start  = entries[0][1]   # 0x7B0
    data_end    = entries[-1][1] + entries[-1][2]  # 0x70990
    total_data  = data_end - data_start            # 0x701e0
    rebuilt_data = bytearray(total_data)

    rebuilt_header = bytearray()
    read_offset = 0  # position in decrypted file (with alignment padding)

    for eid, offset, size, flags in entries:
        # Read plaintext from decrypted file
        plaintext = bytes(data[read_offset:read_offset + size])
        read_offset += align(size)  # skip alignment padding

        # Encrypt
        enc_data, sha1_key, nonce = encrypt_entry(plaintext)

        # Place encrypted blob at correct offset in rebuilt_data
        rel_offset = offset - data_start
        rebuilt_data[rel_offset:rel_offset + size] = enc_data

        # Build entry header
        entry_struct = struct.pack(
            "<IIII12s20s",
            eid, offset, size, flags, nonce, sha1_key
        )
        rebuilt_header += entry_struct

    # Header SHA1 = sha1(encrypted_data + header_nonce)
    header_nonce   = bytearray.fromhex('11' * 16)
    new_header_sha1 = sha1_custom(bytes(rebuilt_data), bytes(header_nonce))

    # File header (0x30 bytes)
    file_header = struct.pack("<III", 0x64, len(entries), 0x06F530)
    file_header += bytes(header_nonce)
    file_header += new_header_sha1

    # Assemble: file_header + all entry headers + encrypted data
    full_data = file_header + rebuilt_header + rebuilt_data

    orig_size = 0x7d000
    padding = orig_size - len(full_data)
    if padding > 0:
        full_data += bytearray(padding)

    with open(file_path + '.enc', 'wb') as f:
        f.write(full_data)

    print(f"Written {hex(len(full_data))} bytes to {file_path}.enc")

if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Save file encrypt/decrypt tool')
    parser.add_argument('mode', choices=['decrypt', 'encrypt'], help='Operation mode')
    parser.add_argument('file', help='Path to the save file')
    args = parser.parse_args()

    if args.mode == 'decrypt':
        decrypt_save(args.file)
    elif args.mode == 'encrypt':
        encrypt_save(args.file)


# for i in range(len(entries)-1):
#     curr_end  = entries[i][1] + entries[i][2]
#     next_start = entries[i+1][1]
#     gap = next_start - curr_end
#     if gap != 0:
#         print(f"Gap between entry {i} and {i+1}: {hex(gap)} bytes (end={hex(curr_end)} next={hex(next_start)})")