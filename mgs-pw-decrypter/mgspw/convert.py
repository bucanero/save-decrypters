"""Moving save data between the PSP and PS3 (HD Edition) releases.

The two formats share their whole main block: header 1, then `BLOCK1_SIZE`
bytes of save data at the same offsets, stored little-endian on both.  So the
conversion is a straight copy of that block into a save from the target
platform, followed by a checksum refresh.

The PS3 additionally carries a second block holding its online/comrade data.
`transfarmer` maps the bytes that follow the main block on the PSP onto that
second block; this module can do the same with `include_block2`, but it is off
by default -- see the README for why that mapping is doubtful.
"""

from .crypto import SaveError
from .layout import (
    BLOCK1_OFF,
    BLOCK1_WORDS,
    BLOCK2_OFF,
    BLOCK2_WORDS,
    HEADER1_WORDS,
    HEADER2_OFF,
    PS3,
    PSP,
)


class Mapping(object):
    """How many words each stage of a conversion moves."""

    def __init__(self, psp_game_words, copy_b1, remaining, copy_b2):
        self.psp_game_words = psp_game_words
        self.copy_b1 = copy_b1
        self.remaining = remaining
        self.copy_b2 = copy_b2

    def __repr__(self):
        return (
            "<Mapping psp_game=%d copy_b1=%d remaining=%d copy_b2=%d>"
            % (self.psp_game_words, self.copy_b1, self.remaining, self.copy_b2)
        )


def plan(psp_len, ps3_len, include_block2=False):
    """Work out the word counts for a conversion between two given saves."""
    psp_words = psp_len // 4
    ps3_words = ps3_len // 4

    psp_game_words = max(psp_words - HEADER1_WORDS, 0)
    copy_b1 = min(psp_game_words, BLOCK1_WORDS)

    remaining = max(psp_game_words - BLOCK1_WORDS, 0)
    copy_b2 = 0
    if include_block2 and remaining:
        ps3_b2_avail = max(ps3_words - (BLOCK2_OFF // 4), 0)
        copy_b2 = min(remaining, ps3_b2_avail, BLOCK2_WORDS)

    return Mapping(psp_game_words, copy_b1, remaining, copy_b2)


def _check(src, dst, want_src, want_dst):
    if src.platform != want_src:
        raise SaveError("source save is %s, expected %s" % (src.platform, want_src))
    if dst.platform != want_dst:
        raise SaveError(
            "target save is %s, expected %s" % (dst.platform, want_dst)
        )


def psp_to_ps3(psp, ps3, include_block2=False):
    """Copy a PSP save's data into a PS3 save, in place. Returns the Mapping."""
    _check(psp, ps3, PSP, PS3)
    mapping = plan(len(psp.data), len(ps3.data), include_block2)

    size = mapping.copy_b1 * 4
    ps3.data[BLOCK1_OFF : BLOCK1_OFF + size] = psp.data[
        BLOCK1_OFF : BLOCK1_OFF + size
    ]

    if mapping.copy_b2:
        size = mapping.copy_b2 * 4
        ps3.data[BLOCK2_OFF : BLOCK2_OFF + size] = psp.data[
            HEADER2_OFF : HEADER2_OFF + size
        ]

    ps3.fix_checksums()
    return mapping


def ps3_to_psp(ps3, psp, include_block2=False):
    """Copy a PS3 save's data into a PSP save, in place. Returns the Mapping."""
    _check(ps3, psp, PS3, PSP)
    mapping = plan(len(psp.data), len(ps3.data), include_block2)

    size = mapping.copy_b1 * 4
    psp.data[BLOCK1_OFF : BLOCK1_OFF + size] = ps3.data[
        BLOCK1_OFF : BLOCK1_OFF + size
    ]

    if mapping.copy_b2:
        size = mapping.copy_b2 * 4
        psp.data[HEADER2_OFF : HEADER2_OFF + size] = ps3.data[
            BLOCK2_OFF : BLOCK2_OFF + size
        ]

    psp.fix_checksums()
    return mapping
