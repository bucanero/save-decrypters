import struct

def next_seed(seed):
    return ((seed << 4) + (seed >> 4)) & 0xFFFFFFFF
    
def decrypt_block(data, offset):

    magic    = struct.unpack_from("<I", data, offset)[0]
    size     = struct.unpack_from("<I", data, offset + 4)[0]
    checksum = struct.unpack_from("<I", data, offset + 8)[0]
    seed     = struct.unpack_from("<I", data, offset + 12)[0]

    section = data[offset:offset+size]
    enc = section[16:]

    calc = sum(enc) & 0xFFFFFFFF

    out = bytearray()

    for i in range(0, len(enc), 4):
        if i + 4 > len(enc):
            break

        word = struct.unpack_from("<I", enc, i)[0]
        out += struct.pack("<I", word ^ seed)
        seed = next_seed(seed)




    return section[:16] + out


def decrypt_block_3(data, offset):

    magic    = struct.unpack_from("<I", data, offset)[0]
    size     = struct.unpack_from("<I", data, offset + 4)[0] # for some reason the size field is all zeors so we assume to end of reamining data
    checksum = struct.unpack_from("<I", data, offset + 8)[0]
    seed     = struct.unpack_from("<I", data, offset + 12)[0]

    
    section = data[offset:]
    print(hex(offset), ' '.join(f'{b:02X}' for b in section[:0x10]))
    enc = section[16:]

    calc = sum(enc) & 0xFFFFFFFF

    out = bytearray()

    for i in range(0, len(enc), 4):
        if i + 4 > len(enc):
            break

        word = struct.unpack_from("<I", enc, i)[0]
        out += struct.pack("<I", word ^ seed)
        seed = next_seed(seed)

    return section[:16] + out


def calc_checksum(data):
    return sum(data) & 0xFFFFFFFF



def decrypt_save(data):

    block1= decrypt_block(data, 0)
    block2= decrypt_block(data, 0x106010)
    block3= decrypt_block_3(data, 0x213B30)

    data_dec = block1 + block2 + block3

    with open('SAVESYS.DAT_decrypted', "wb") as f:
        f.write(data_dec)

def encrypt_save(data):

    block1= decrypt_block(data, 0)

    block1_check=calc_checksum(block1[16:])

    struct.pack_into('<I', block1, 8, block1_check)
    
    block2= decrypt_block(data, 0x106010)

    block2_check=calc_checksum(block2[16:])

    struct.pack_into('<I', block2, 8, block2_check)


    block3= decrypt_block_3(data, 0x213B30)

    block3_check=calc_checksum(block3[16:])

    struct.pack_into('<I', block3, 8, block3_check)

    data_enc = block1 + block2 + block3

    with open('SAVESYS.DAT_encrypted', "wb") as f:
        f.write(data_enc)



with open('SAVESYS.DAT_decrypted', "rb") as f:
    data = bytearray(f.read())

encrypt_save(data)