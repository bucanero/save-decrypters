import argparse
import hashlib
import os
import struct
from Crypto.Cipher import AES


class CryptoError(Exception):
    pass


class Crypt_Xeno2:
    SAVE_HEADER_KEY = b"PR]-<Q9*WxHsV8rcW!JuH7k_ug:T5ApX"
    SAVE_HEADER_INITIAL_VALUE = b"_Y7]mD1ziyH#Ar=0"
    SAVE_MAGIC_HEADER = b"H\x89\x01L"
    INTERNAL_KEY_OFFSETS = (0x1C, 0x4C)
    HEADER = b"#SAV"

    @staticmethod
    def encryptFile(filePath: str) -> bytes:
        with open(filePath, "rb") as decrypted_file:
            decrypted_file.seek(-1, 2)
            decrypted_file_sub1 = decrypted_file.tell() - 0x80

            key_offset = bytearray(decrypted_file.read(1))[0]
            if key_offset not in Crypt_Xeno2.INTERNAL_KEY_OFFSETS:
                raise CryptoError("KEY OFFSET NOT FOUND!")

            decrypted_file.seek(key_offset)
            key = decrypted_file.read(0x20)
            initial_value = decrypted_file.read(0x10)
            new_key = AES.new(key, AES.MODE_CTR, initial_value=initial_value, nonce=b"")
            decrypted_file.seek(0)

            enc_header = AES.new(
                Crypt_Xeno2.SAVE_HEADER_KEY,
                AES.MODE_CTR,
                initial_value=Crypt_Xeno2.SAVE_HEADER_INITIAL_VALUE,
                nonce=b"",
            ).decrypt(decrypted_file.read(0x80))
            enc_save = new_key.encrypt(
                decrypted_file.read(decrypted_file_sub1) + b"\x00"
            )

        md5_hash = hashlib.md5()
        md5_hash.update(enc_header)
        md5_hash.update(enc_save)

        encrypted_bytes = (
            Crypt_Xeno2.SAVE_MAGIC_HEADER
            + struct.pack("<i", len(enc_save) + len(enc_header) + 0x20)
            + struct.pack("<i", len(enc_save) + len(enc_header))
            + b"\x00\x00\x00\x00"
            + md5_hash.digest()
            + enc_header
            + enc_save
        )
        return encrypted_bytes

    @staticmethod
    def decryptFile(filePath: str) -> bytes:
        with open(filePath, "rb") as encrypted_file:
            if encrypted_file.read(4) != Crypt_Xeno2.SAVE_MAGIC_HEADER:
                raise CryptoError("INCORRECT HEADER!")

            encrypted_file.seek(0x10)
            original_hash = encrypted_file.read(0x10)

            md5_hash = hashlib.md5()
            md5_hash.update(encrypted_file.read())
            if md5_hash.digest() != original_hash:
                raise CryptoError("MD5 HASH MISMATCH!")
            encrypted_file.seek(0x20)

            dec_header = AES.new(
                Crypt_Xeno2.SAVE_HEADER_KEY,
                AES.MODE_CTR,
                initial_value=Crypt_Xeno2.SAVE_HEADER_INITIAL_VALUE,
                nonce=b"",
            ).decrypt(encrypted_file.read(0x80))
            enc_data_test = encrypted_file.read(16)

            for key_offset in Crypt_Xeno2.INTERNAL_KEY_OFFSETS:
                key = dec_header[key_offset : key_offset + 0x20]
                initial_value = dec_header[
                    key_offset + 0x20 : key_offset + (0x20 + 0x10)
                ]
                new_key = AES.new(
                    key, AES.MODE_CTR, initial_value=initial_value, nonce=b""
                )
                test_data = new_key.decrypt(enc_data_test)

                if test_data.startswith(Crypt_Xeno2.HEADER):
                    break
            else:
                raise CryptoError("INTERNAL SAVE ENCRYPTION KEY NOT FOUND!")

            encrypted_file.seek(0x20 + 0x80)
            new_key = AES.new(key, AES.MODE_CTR, initial_value=initial_value, nonce=b"")
            decrypted_save = bytearray(new_key.decrypt(encrypted_file.read()))
            decrypted_save[-1] = key_offset
            decrypted_total = dec_header + decrypted_save
        return decrypted_total


def calculate_checksum(data, start_offset):
    if start_offset >= len(data):
        return None
    section2 = data[start_offset:]
    checksum = 0
    num_blocks = len(section2) // 0x20
    for i in range(num_blocks):
        checksum += section2[i * 0x20]
        checksum &= 0xFF
    return checksum


def checksum8(data):
    checksum = calculate_checksum(data, 0x80)
    if checksum is not None:
        print(f"Checksum8: {hex(checksum)}")
    return checksum


def checksum2(data):
    checksum = calculate_checksum(data, 0xA0)
    if checksum is not None:
        print(f"Checksum2: {hex(checksum)}")
    return checksum


def checksum3(data):
    if len(data) < 0x80:
        return None
    checksum = (data[0x6C] + data[0x70] + data[0x74] + data[0x78]) & 0xFF
    print(f"Checksum3: {hex(checksum)}")
    return checksum


def checksum4(data):
    if len(data) < 0x80:
        return None
    checksum = (data[0x3C] + data[0x40] + data[0x44] + data[0x48]) & 0xFF
    print(f"Checksum4: {hex(checksum)}")
    return checksum


def checksum5(data):
    if len(data) < 0x80:
        return None
    checksum = 0
    for i in range(8):
        checksum += data[0x4C + i * 4]
        checksum &= 0xFF
    print(f"Checksum5: {hex(checksum)}")
    return checksum


def checksum6(data):
    if len(data) < 0x40:
        return None
    checksum = 0
    for i in range(8):
        checksum += data[0x1C + i * 4]
        checksum &= 0xFF
    print(f"Checksum6: {hex(checksum)}")
    return checksum


def checksum7(data):
    if len(data) < 0x14:
        return None
    checksum = 0
    for i in range(14):
        checksum += data[0x06 + i]
        checksum &= 0xFF
    print(f"Checksum7: {hex(checksum)}")
    return checksum


def checksum1(data):
    checksum = (
        data[0x05]
        + data[0x15]
        + data[0x16]
        + data[0x17]
        + data[0x18]
        + data[0x19]
        + data[0x1A]
        + data[0x1B]
    )
    checksum &= 0xFF
    print(f"Checksum1: {hex(checksum)}")
    return checksum


def calc_checksum(file_path):
    with open(file_path, "rb") as f:
        test_data = f.read()

    expected_header = bytes.fromhex("23 53 41 56")
    is_decrypted = test_data[:4] == expected_header

    if not is_decrypted:
        dec_save = Crypt_Xeno2.decryptFile(file_path)
        if dec_save is None:
            print("Error: Decryption failed!")
            return
    else:
        dec_save = test_data

    check8 = checksum8(dec_save)

    if not is_decrypted:
        check2 = checksum2(test_data)
    else:
        temp_path = file_path + ".tmp_for_enc"
        with open(temp_path, "wb") as tf:
            tf.write(dec_save)
        enc_data = Crypt_Xeno2.encryptFile(temp_path)
        check2 = checksum2(enc_data)

    check3 = checksum3(dec_save)
    check4 = checksum4(dec_save)
    check5 = checksum5(dec_save)
    check6 = checksum6(dec_save)
    check7 = checksum7(dec_save)

    dec_save = bytearray(dec_save)
    dec_save[0x15] = check7
    dec_save[0x16] = check6
    dec_save[0x17] = check5
    dec_save[0x18] = check4
    dec_save[0x19] = check3
    dec_save[0x1B] = check2
    dec_save[0x1A] = check8

    check1 = checksum1(dec_save)
    dec_save[0x14] = check1
    dec_save_bytes = bytes(dec_save)

    if not is_decrypted:
        out_path = file_path + ".dec"
        with open(out_path, "wb") as f:
            f.write(dec_save_bytes)
        print("Decrypted file saved as:", out_path)
    else:
        temp_path = file_path + ".tmp_for_enc"
        try:
            with open(temp_path, "wb") as tf:
                tf.write(dec_save_bytes)
            enc_save_bytes = Crypt_Xeno2.encryptFile(temp_path)
        finally:
            try:
                os.remove(temp_path)
            except OSError:
                pass

        out_path = file_path + ".enc"
        with open(out_path, "wb") as f:
            f.write(enc_save_bytes)
        print("Encrypted file saved as:", out_path)

    print("Checksum calculation and file updates complete.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Xeno2 Save Checksum CLI Tool")
    parser.add_argument("file", help="Path to the save file")
    args = parser.parse_args()
    calc_checksum(args.file)
