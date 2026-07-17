#!/usr/bin/env python3
"""
Shadow of War — SYSTEM.DAT Checksum Fixer

Header layout (20 bytes):
  [0:4]   Magic:    b'SAVE'
  [4:8]   Version:  uint32 LE
  [8:12]  Checksum: uint32 LE — sum of all bytes from offset 20 to EOF
  [12:16] Size:     uint32 LE — file size minus 20
  [16:20] Size:     uint32 LE — same as above

Usage:
  python fix_checksum.py SYSTEM.DAT              # verify only
  python fix_checksum.py SYSTEM.DAT --fix        # recalculate and write
  python fix_checksum.py SYSTEM.DAT --fix --backup  # fix + keep .bak copy
"""

import struct
import sys
import shutil
from pathlib import Path

HEADER_SIZE = 20
CHECKSUM_OFFSET = 8
MAGIC = b"SAVE"


def read_save(path: Path) -> bytearray:
    data = bytearray(path.read_bytes())
    if data[:4] != MAGIC:
        sys.exit(f"Error: {path} is not a valid Shadow of War save (missing SAVE magic).")
    if len(data) < HEADER_SIZE:
        sys.exit(f"Error: file too small ({len(data)} bytes).")
    return data


def calc_checksum(data: bytearray) -> int:
    return sum(data[HEADER_SIZE:]) & 0xFFFFFFFF


def get_stored_checksum(data: bytearray) -> int:
    return struct.unpack_from("<I", data, CHECKSUM_OFFSET)[0]


def set_checksum(data: bytearray, value: int):
    struct.pack_into("<I", data, CHECKSUM_OFFSET, value)


def main():
    if len(sys.argv) < 2:
        print(__doc__.strip())
        sys.exit(1)

    path = Path(sys.argv[1])
    do_fix = "--fix" in sys.argv
    do_backup = "--backup" in sys.argv

    data = read_save(path)

    stored = get_stored_checksum(data)
    calculated = calc_checksum(data)

    print(f"File:       {path}  ({len(data):,} bytes)")
    print(f"Stored:     0x{stored:08X}")
    print(f"Calculated: 0x{calculated:08X}")

    if stored == calculated:
        print("Status:     ✓ checksum OK")
    else:
        print("Status:     ✗ MISMATCH")

    if do_fix:
        if stored == calculated:
            print("\nNo fix needed.")
            return

        if do_backup:
            bak = path.with_suffix(path.suffix + ".bak")
            shutil.copy2(path, bak)
            print(f"\nBackup:     {bak}")

        set_checksum(data, calculated)
        path.write_bytes(data)
        print(f"Fixed:      wrote 0x{calculated:08X} to offset {CHECKSUM_OFFSET}")
    elif stored != calculated:
        print("\nRun with --fix to update the checksum (add --backup to keep a copy).")


if __name__ == "__main__":
    main()
