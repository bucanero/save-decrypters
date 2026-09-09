"""The Metal Gear Solid: Peace Walker save cipher.

Each encrypted block is XORed with the output of a linear congruential
generator whose seed is derived from a 16-word plain-text header sitting in
front of the block.  The header word at index 1, ORed with a constant and
XORed with word 0, yields a small index; three words at that index supply the
seed material.

The decrypted files this module produces are byte-for-byte identical to the
ones produced by `mgs-pw-decrypter` in this repository, so the two tools are
interchangeable.
"""

import struct

from ._table import CSUM_TABLE
from .layout import (
    BLOCK1_OFF,
    BLOCK2_CSUM_WORD,
    BLOCK2_OFF,
    BLOCK2_SIZE,
    BLOCK2_WORDS,
    HEADER1_WORDS,
    HEADER2_OFF,
    MIN_SIZE,
    PS3,
    PS3_MIN_SIZE,
    PSP,
    PSP_LAYOUTS,
    REGION1_STANDARD,
    block1_size,
    checksum_ranges,
)

MASK = 0xFFFFFFFF
LCG_MULT = 0x2E90EDD
HEADER_OR = 0xAD47DE8F
SEED_XOR_R5 = 0xBC4DEFA2
SEED_XOR_R4 = 0x2D71D26C
SEED_XOR_R3 = 0x1327DE73
SALT_MIX = 0x6576

LE = "<"
BE = ">"

_T0 = CSUM_TABLE[0]


class SaveError(Exception):
    """The file is not a usable Peace Walker save."""


def checksum(data):
    """The game's custom integrity checksum over a byte range."""
    table = CSUM_TABLE
    t0 = _T0
    csum = MASK
    for byte in data:
        csum = (table[(byte ^ csum) & 0xFF] ^ (csum >> 8) ^ t0) & MASK
    return ~csum & MASK


def _seed_index(w0, w1):
    return ((w1 | HEADER_OR) ^ w0) & MASK


def _salts(buf, base, endian):
    """Derive (s0, s1) from the 16-word header at `base`."""
    header = struct.unpack_from(endian + "%dI" % HEADER1_WORDS, buf, base)
    index = _seed_index(header[0], header[1])
    if index + 7 >= HEADER1_WORDS:
        raise SaveError(
            "seed index 0x%X out of range at offset 0x%X -- not a Peace Walker save"
            % (index, base)
        )
    r5 = header[index + 7] ^ SEED_XOR_R5
    r4 = header[index + 3] ^ SEED_XOR_R4
    r3 = header[index + 2] ^ SEED_XOR_R3

    s0 = (r3 ^ r4) & MASK
    s1 = (s0 * r5) & MASK
    s0 = (((s0 ^ SALT_MIX) << 16) | s0) & MASK
    return s0, s1


def _xor_block(buf, off, nwords, salts, endian):
    """XOR `nwords` words at `off` with the keystream. Self-inverse."""
    fmt = endian + "%dI" % nwords
    words = list(struct.unpack_from(fmt, buf, off))
    s0, s1 = salts
    for i in range(nwords):
        words[i] ^= s0
        s0 = (s0 * LCG_MULT + s1) & MASK
    struct.pack_into(fmt, buf, off, *words)


def _swap_words(buf, off, nwords):
    """Reverse the bytes of each of `nwords` words at `off`."""
    words = struct.unpack_from("<%dI" % nwords, buf, off)
    struct.pack_into(">%dI" % nwords, buf, off, *words)


def has_block2(buf):
    """Whether the save carries the PS3-only second block.

    Header 2 is plain text and is never touched by either the cipher or the
    swaps, so this answers the same on an encrypted and a decrypted file.
    """
    if len(buf) < PS3_MIN_SIZE:
        return False
    w0, w1 = struct.unpack_from(">2I", buf, HEADER2_OFF)
    return _seed_index(w0, w1) < 0x10


def detect(buf):
    """Return PS3 or PSP, or raise if the file cannot be a Peace Walker save."""
    if len(buf) < MIN_SIZE:
        raise SaveError(
            "file is 0x%X bytes, need at least 0x%X" % (len(buf), MIN_SIZE)
        )
    return PS3 if has_block2(buf) else PSP


def layout_for(buf, platform=None):
    """The first-region length that describes this save's layout.

    A PS3 save always uses the standard layout.  For a PSP save the size
    settles it, since the known variants differ in total size by exactly the
    amount their first region differs by.  An unrecognised size falls back to
    the standard layout, and the checksum check will say so if that is wrong.
    """
    if platform is None:
        platform = detect(buf)
    if platform == PS3:
        return REGION1_STANDARD
    return PSP_LAYOUTS.get(len(buf), REGION1_STANDARD)


def header_endian(platform, decrypted):
    """Byte order of header 1 in a save.

    In a file as it sits on disk the header is little-endian on both
    platforms.  A decrypted save keeps it in the platform's native order, so
    a decrypted PS3 header reads big-endian.
    """
    return BE if (decrypted and platform == PS3) else LE


def verify(buf, platform, hdr_endian=LE, region1=None):
    """Check the stored checksums of a decrypted save.

    Returns the list of range starts whose checksum does not match.
    `hdr_endian` is the byte order of header 1 -- see `header_endian`.
    """
    if region1 is None:
        region1 = layout_for(buf, platform)
    bad = []
    for index, off, length in checksum_ranges(region1):
        stored = struct.unpack_from(hdr_endian + "I", buf, index * 4)[0]
        if checksum(buf[off : off + length]) != stored:
            bad.append(off)
    if platform == PS3:
        stored = struct.unpack_from(">I", buf, BLOCK2_CSUM_WORD * 4)[0]
        if checksum(buf[BLOCK2_OFF : BLOCK2_OFF + BLOCK2_SIZE]) != stored:
            bad.append(BLOCK2_OFF)
    return bad


def write_checksums(buf, platform, hdr_endian=LE, region1=None):
    """Recompute and store every checksum in a decrypted save."""
    if region1 is None:
        region1 = layout_for(buf, platform)
    for index, off, length in checksum_ranges(region1):
        struct.pack_into(
            hdr_endian + "I", buf, index * 4, checksum(buf[off : off + length])
        )
    if platform == PS3:
        struct.pack_into(
            ">I",
            buf,
            BLOCK2_CSUM_WORD * 4,
            checksum(buf[BLOCK2_OFF : BLOCK2_OFF + BLOCK2_SIZE]),
        )


def decrypt(buf):
    """Decrypt in place. Returns (platform, list of bad checksum offsets)."""
    platform = detect(buf)
    region1 = layout_for(buf, platform)

    _xor_block(
        buf, BLOCK1_OFF, block1_size(region1) // 4, _salts(buf, 0, LE), LE
    )
    if platform == PS3:
        _xor_block(
            buf, BLOCK2_OFF, BLOCK2_WORDS, _salts(buf, HEADER2_OFF, BE), BE
        )

    bad = verify(buf, platform, LE, region1)

    # The decrypted header is left in the save's native byte order: the PS3
    # reads it big-endian, the PSP little-endian (which is the file order, so
    # the PSP needs no swap).
    if platform == PS3:
        _swap_words(buf, 0, HEADER1_WORDS + 1)

    return platform, bad


def encrypt(buf):
    """Re-encrypt in place, refreshing the checksums. Returns the platform."""
    platform = detect(buf)
    region1 = layout_for(buf, platform)

    if platform == PS3:
        _swap_words(buf, 0, HEADER1_WORDS + 1)

    write_checksums(buf, platform, LE, region1)

    if platform == PS3:
        _xor_block(
            buf, BLOCK2_OFF, BLOCK2_WORDS, _salts(buf, HEADER2_OFF, BE), BE
        )
    _xor_block(
        buf, BLOCK1_OFF, block1_size(region1) // 4, _salts(buf, 0, LE), LE
    )

    return platform
