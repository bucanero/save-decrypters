#!/usr/bin/env python3
"""
Goat Simulator 3 - PS4 Save Decrypt/Re-encrypt Tool
Game: CUSA46680

HOW THE SAVE IS STRUCTURED
---------------------------
The PS4 OS-level encryption is already stripped by the time you have the file
(Apollo / SaveWizard / FTP dump handles that layer).

What's left is a second application-level layer the game adds itself:

  Bytes 0-7   : Magic constant  0x1a4777924f98f282  (plaintext, little-endian)
  Bytes 8-11  : Uncompressed size (little-endian uint32)
  Bytes 12-15 : Ciphertext size  (little-endian uint32)
  Bytes 16+   : AES-256-ECB encrypted payload

The encrypted payload decrypts to a zlib-compressed UE4 GVAS binary.

CIPHER DETAILS (from EBOOT.ELF reverse engineering)
-----------------------------------------------------
  Algorithm : AES-256-ECB  (no IV, no CBC chaining)
  Key       : hardcoded 32 bytes at VA 0x06efa6d0 in EBOOT.ELF
              (file offset 0x6efe6d0)
  Padding   : PKCS7 to 16-byte boundary
  After AES : zlib decompress -> raw GVAS

The AES implementation is a custom 4-table lookup (not OpenSSL).
Key schedule lives in FUN_02f3b890, encrypt in FUN_023af540,
decrypt in FUN_02f3b9e0.

REQUIREMENTS
------------
  pip install pycryptodome
"""

import struct
import zlib
import sys
from Crypto.Cipher import AES

# Hardcoded AES-256 key from EBOOT.ELF (VA 0x06efa6d0)
KEY = bytes.fromhex(
    '4c0854713a51a104'
    'fe9b1f752275d236'
    '4f600644b6de4f54'
    '73db5b92273ec0af'
)

# Save file magic (first 8 bytes, stored little-endian)
MAGIC = 0x82f2984f9277471a  # = 0x1a4777924f98f282 as bytes in file


def decrypt(path_in, path_out):
    """
    Decrypt a GS3 PS4 save file -> raw GVAS binary.

    path_in  : ue4savegame.ps4.sav  (after PS4 OS layer is stripped)
    path_out : output .gvas file (standard UE4 save, edit with any UE4 save tool)
    """
    data = open(path_in, 'rb').read()

    # Verify magic
    magic = struct.unpack_from('<Q', data, 0)[0]
    if magic != MAGIC:
        raise ValueError(f"Not a GS3 save - bad magic: {magic:#018x}")

    decomp_size = struct.unpack_from('<I', data, 8)[0]   # expected size after zlib decompress
    cipher_size = struct.unpack_from('<I', data, 12)[0]  # AES ciphertext byte count
    ciphertext  = data[16:16 + cipher_size]

    # AES-256-ECB decrypt
    plain = AES.new(KEY, AES.MODE_ECB).decrypt(ciphertext)

    # Strip PKCS7 padding (last byte tells you how many pad bytes to remove)
    pad   = plain[-1]
    plain = plain[:-pad]

    # zlib decompress -> GVAS
    out = zlib.decompress(plain)
    assert len(out) == decomp_size, f"Decompress size mismatch: {len(out)} != {decomp_size}"

    open(path_out, 'wb').write(out)
    print(f"[+] Decrypted: {path_out}  ({len(out)} bytes)")


def encrypt(path_in, path_out):
    """
    Re-encrypt a modified GVAS back into a GS3 PS4 save file.

    path_in  : modified .gvas file
    path_out : ue4savegame.ps4.sav  (drop back into the save folder on PS4)
    """
    plain = open(path_in, 'rb').read()
    decomp_size = len(plain)

    # zlib compress
    compressed = zlib.compress(plain, level=6)

    # PKCS7 pad to 16-byte boundary
    pad        = 16 - (len(compressed) % 16)
    compressed += bytes([pad]) * pad

    cipher_size = len(compressed)

    # AES-256-ECB encrypt
    ciphertext = AES.new(KEY, AES.MODE_ECB).encrypt(compressed)

    # Write header + ciphertext
    header = struct.pack('<QII', MAGIC, decomp_size, cipher_size)
    open(path_out, 'wb').write(header + ciphertext)
    print(f"[+] Encrypted: {path_out}  ({len(header + ciphertext)} bytes)")


if __name__ == '__main__':
    if len(sys.argv) != 4 or sys.argv[1] not in ('decrypt', 'encrypt'):
        print(__doc__)
        print("Usage:")
        print("  python gs3_save_tool.py decrypt  ue4savegame.ps4.sav  out.gvas")
        print("  python gs3_save_tool.py encrypt  modified.gvas         ue4savegame.ps4.sav")
        sys.exit(1)

    cmd = sys.argv[1]
    if cmd == 'decrypt':
        decrypt(sys.argv[2], sys.argv[3])
    else:
        encrypt(sys.argv[2], sys.argv[3])
