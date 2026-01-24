from io import BytesIO
import struct
import argparse
import re

from Crypto.Cipher import AES

GTA5_AND_REDDEAD_KEY = AES.new(b'\x16\x85\xff\xa3\x8d\x01\x0f\r\xfef\x1c\xf9\xb5W,P\r\x80&H\xdb7\xb9\xed\x0fH\xc5sB\xc0"\xf5', AES.MODE_ECB)

def rdr2_checksum(data: bytes, /) -> bytes:
    checksum = 0x3fac7125

    for char in data:
        char = (char + 128) % 256 - 128 # casting to signed char
        checksum = ((char + checksum) * 0x401) & 0xFFFFFFFF
        checksum = (checksum >> 6 ^ checksum) & 0xFFFFFFFF
    checksum = (checksum*9) & 0xFFFFFFFF
    
    return struct.pack('>I',((checksum >> 11 ^ checksum) * 0x8001) & 0xFFFFFFFF)

def _crypt_rdr2_ps4_save(rdr2_save: BytesIO,/,*,enc_data_offset: int,do_enc: bool = True) -> bytes:
    previous_spot = rdr2_save.tell()
    rdr2_save.seek(0)
    rdr2_save.seek(enc_data_offset)

    crypted_data = rdr2_save.read()
    
    rdr2_save.seek(0)
    if enc_data_offset == 0x120 and do_enc:
        for chunk in [m.start() for m in re.finditer(b'CHKS\x00', crypted_data)]: # calculate checksums for each chunk
            rdr2_save.seek(enc_data_offset + chunk + 4,0) # 4 bytes for the magic CHKS
            header_size = struct.unpack('>I',rdr2_save.read(4))[0]
            data_length = struct.unpack('>I',rdr2_save.read(4))[0]
            rdr2_save.seek(header_size - 4 - 4 - 4,1) # 4 for the header size num, 4 for the data length num AND 4 for the checksum

            rdr2_save.seek(-data_length,1)
            data_to_be_hashed = bytearray(rdr2_save.read(data_length))

            chks_offset = len(data_to_be_hashed)-header_size + 4 + 4
            data_to_be_hashed[chks_offset:chks_offset + 8] = b'\x00' * 8 # remove the length and hash
            new_hash = rdr2_checksum(data_to_be_hashed)
            
            rdr2_save.seek(enc_data_offset + chunk + 12,0)
            rdr2_save.write(new_hash)
            rdr2_save.seek(0)
    rdr2_save.seek(enc_data_offset)
    crypted_data = rdr2_save.read()
    rdr2_save.seek(0)
    if do_enc:
        dec_data = GTA5_AND_REDDEAD_KEY.encrypt(crypted_data)
    else:
        dec_data = GTA5_AND_REDDEAD_KEY.decrypt(crypted_data)
    
    result = rdr2_save.read(enc_data_offset) + dec_data
    rdr2_save.seek(previous_spot)
        
    return result


def encrypt_rdr2_ps4_save(rdr2_save: BytesIO,/,*,enc_data_offset: int) -> bytes:
    return _crypt_rdr2_ps4_save(rdr2_save,enc_data_offset=enc_data_offset,do_enc=True)


def decrypt_rdr2_ps4_save(rdr2_save: BytesIO,/,*,enc_data_offset: int) -> bytes:
    return _crypt_rdr2_ps4_save(rdr2_save,enc_data_offset=enc_data_offset,do_enc=False)


def auto_encrypt_decrypt(rdr2_save: BytesIO,/) -> bytes:
    previous_spot = rdr2_save.tell()
    rdr2_save.seek(0)
    rdr2_save.seek(0x114)
    if rdr2_save.read(4) == b'\x00\x00\x00\x00':
        ENC_DATA_OFFSET = 0x120
        DEC_SAVE_HEADER = b'RSAV'
    else:
        ENC_DATA_OFFSET = 0x114
        DEC_SAVE_HEADER = b'PSIN'

    rdr2_save.seek(0)
    rdr2_save.seek(ENC_DATA_OFFSET)
    
    header = rdr2_save.read(4)
    rdr2_save.seek(previous_spot)
    
    if header == DEC_SAVE_HEADER:
        return encrypt_rdr2_ps4_save(rdr2_save,enc_data_offset=ENC_DATA_OFFSET)
    else:
        return decrypt_rdr2_ps4_save(rdr2_save,enc_data_offset=ENC_DATA_OFFSET)

def main():
    
    # from zlib import crc32
    # from pathlib import Path
    
    # print(hex(crc32(Path('Untitled1.bin').read_bytes())))
    
    
    # return
    parser = argparse.ArgumentParser(description='Simple tool to encrypt and decrypt ps4 saves for Red Dead Redemption 2 and GTAV, detects if its encrypted or decrypted automatically')
    
    parser.add_argument("input_save", help="Path to the save file")
    parser.add_argument("output_save", help="Path to write the output save")
    
    args = parser.parse_args()
    
    with open(args.input_save,'rb') as f:
        old_data = BytesIO(f.read())
    with open(args.output_save,'wb') as f:
        f.write(auto_encrypt_decrypt(old_data))


if __name__ == '__main__':
    main()
