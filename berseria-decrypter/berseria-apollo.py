#
# Tales of Berseria (PS3/PS4/PC) decrypter & checksum fixer by Bucanero
# based on tob.py by https://github.com/alfizari
#

import gc
import ucrypto
import uhashlib
import ubinascii
import ustruct as struct

ENCRYPT = ucrypto.ENCRYPT
DECRYPT = ucrypto.DECRYPT

HASH_POS = 0x001C
HASH_SALT = 0x000C
HASH_START = 0x0034

class ToB_Entry:
    def __init__(self, id=0, off=0, size=0, salt=None, key=None):
        self.item_id = id
        self.dataOffset = off
        self.dataSize = size
        self.salt = salt
        self.keySHA1 = key

def sha1_custom(msg, salt):
    """Custom SHA1 function that concatenates the message and salt before hashing"""

    return uhashlib.sha1(msg + salt)

def read32_val(buf, off, isPS3):
    fmt = '>I' if isPS3 else '<I'
    ret = struct.unpack_from(fmt, buf, off)[0]
    return ret

def decrypt_entry(entry, entry_data):
    # decrypt the entry data in-place
    for i in range(entry.dataSize):
        entry_data[entry.dataOffset + i] ^= entry.keySHA1[i % 20] ^ (i & 0xFF)

    # check integrity of decrypted data
    if (sha1_custom(entry_data[entry.dataOffset:entry.dataOffset+entry.dataSize], entry.salt) != entry.keySHA1):
        print("[!] Warning: Entry ID %08X failed integrity check!\n", entry.item_id)

def encrypt_entry(entry, entry_data):
    # generate new SHA1 key based on current plaintext data
    entry.keySHA1 = sha1_custom(entry_data[entry.dataOffset:entry.dataOffset+entry.dataSize], entry.salt)

    # encrypt the entry data in-place
    for i in range(entry.dataSize):
        entry_data[entry.dataOffset + i] ^= entry.keySHA1[i % 20] ^ (i & 0xFF)

def decrypt_data(data, entry_count, isPS3):
    entry = ToB_Entry()
    fmt = ">IIII12s20s" if isPS3 else "<IIII12s20s"

    print("[*] Decrypting Entries...")
    for i in range(entry_count):
        entry.item_id, entry.dataOffset, entry.dataSize, flags, entry.salt, entry.keySHA1 = struct.unpack_from(fmt, data, 0x30 + i * 0x30)
        decrypt_entry(entry, data)

    return

def encrypt_data(data, entry_count, isPS3):
    entry = ToB_Entry()
    fmt = ">IIII12s20s" if isPS3 else "<IIII12s20s"

    print("[*] Encrypting Entries...")
    for i in range(entry_count):
        entry.item_id, entry.dataOffset, entry.dataSize, flags, entry.salt, entry.keySHA1 = struct.unpack_from(fmt, data, 0x30 + i * 0x30)
        encrypt_entry(entry, data)

        struct.pack_into(fmt, data, 0x30 + i * 0x30, entry.item_id, entry.dataOffset, entry.dataSize, flags, entry.salt, entry.keySHA1)

    return

def berseria_crypt(mode, data: bytearray):
    """Main function to handle decryption/encryption of the save file"""

    isPS3 = (data[:7] == b"\x00\x00\x00\x64\x00\x00\x00")
    magic = read32_val(data, 0x00, isPS3)
    entry_count = read32_val(data, 0x04, isPS3)
    data_size   = read32_val(data, 0x08, isPS3)
    data_start  = read32_val(data, HASH_START, isPS3)
    print("Magic:       {:08X}".format(magic))
    print("Entries:     {}".format(entry_count))
    print("Data size:   {} bytes".format(data_size))

    if (not isPS3) and (magic != 0x64):
        raise Exception("Error: Invalid magic, expected 0x64, got 0x{:08X}".format(magic))

    if data_size > len(data):
        raise Exception("Error: Save size mismatch, expected {}, got {}".format(len(data), data_size))

    if mode == DECRYPT:
        if data[HASH_POS:HASH_POS+20] != sha1_custom(data[data_start:data_size], data[HASH_SALT:HASH_SALT+16]):
            print("Warning: Hash mismatch, expected {}".format(ubinascii.hexlify(data[HASH_POS:HASH_POS+20])))

        decrypt_data(data, entry_count, isPS3)
        print("Decryption complete, {} bytes".format(len(data)))

    elif mode == ENCRYPT:
        encrypt_data(data, entry_count, isPS3)

        data[HASH_POS:HASH_POS+20] = sha1_custom(data[data_start:data_size], data[HASH_SALT:HASH_SALT+16])
        print("New SHA1:    {}".format(ubinascii.hexlify(data[HASH_POS:HASH_POS+20])))
        print("Encryption complete, {} bytes".format(len(data)))

    else:
        raise Exception("Error: Unknown mode '{}', must be 'ENCRYPT' or 'DECRYPT'".format(mode))

    # Clean up
    gc.collect()
