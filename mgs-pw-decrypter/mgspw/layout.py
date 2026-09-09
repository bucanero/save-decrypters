"""Save layout constants for Metal Gear Solid: Peace Walker.

All offsets are byte offsets into the save file unless a name says ``WORDS``,
in which case the unit is 32-bit words.

The PS3 (HD Edition) save is the PSP save format with a second, PS3-only
block appended.  The shared part -- everything the game actually checksums --
is byte-for-byte the same layout on both platforms, and is stored
little-endian on both: the HD port kept the PSP's save structure wholesale.
Only the appended second block is big-endian, because it was written fresh
for the PS3 release.
"""

PS3 = "PS3"
PSP = "PSP"

# --- header 1 and the main block (shared by both platforms) -----------------

HEADER1_OFF = 0x0
HEADER1_WORDS = 16

BLOCK1_OFF = 0x40

# There is more than one save layout.  Every release we have seen agrees on
# everything except the length of the first checksummed region; a release with
# a shorter first region has every later boundary shifted down to match, and
# the whole save is that much smaller.  One number therefore pins the layout.
REGION1_STANDARD = 0x1AF24  # PSP US/EU (ULUS10509, ULES01372) and every PS3
REGION1_COMPACT = 0x1AF14   # PSP JP digital (NPJH50045), 0x10 shorter

REGION2_SIZE = 0x1C00
REGION3_SIZE = 0x18E68
# The main block runs 8 bytes past the last checksummed byte on every sample.
BLOCK1_TAIL = 8
# Bytes after the main block in a PSP save, constant across the variants.
PSP_TRAILER_SIZE = 0x7FF8


def block1_size(region1):
    """Size of the encrypted main block for a layout."""
    return 0x44 + region1 + REGION2_SIZE + REGION3_SIZE + BLOCK1_TAIL - BLOCK1_OFF


def checksum_ranges(region1):
    """(header word index, range start, length) for a layout."""
    region2_off = 0x44 + region1
    region3_off = region2_off + REGION2_SIZE
    return (
        (12, region3_off, REGION3_SIZE),
        (14, 0x00044, region1),
        (15, region2_off, REGION2_SIZE),
    )


def psp_size(region1):
    """Total size of a decrypted PSP save for a layout."""
    return BLOCK1_OFF + block1_size(region1) + PSP_TRAILER_SIZE


# Decrypted PSP save size -> layout.  A PS3 save always uses the standard one.
PSP_LAYOUTS = {
    psp_size(REGION1_STANDARD): REGION1_STANDARD,  # 0x3D9D0
    psp_size(REGION1_COMPACT): REGION1_COMPACT,    # 0x3D9C0
}

# Kept for callers that only care about the common layout.
BLOCK1_SIZE = block1_size(REGION1_STANDARD)  # 0x35998
BLOCK1_WORDS = BLOCK1_SIZE // 4  # 0xD666
CHECKSUMS = checksum_ranges(REGION1_STANDARD)

# --- header 2 and the second block (PS3 only) -------------------------------

HEADER2_OFF = 0x359D8
BLOCK2_OFF = 0x35A18
BLOCK2_SIZE = 0xF0D0
BLOCK2_WORDS = BLOCK2_SIZE // 4  # 0x3C34
BLOCK2_CSUM_WORD = 0xD683  # relative to the start of the file

# Smallest file that can hold the second block, and so the smallest a PS3
# save can be.  A PSP save is always shorter than this.
PS3_MIN_SIZE = BLOCK2_OFF + BLOCK2_SIZE  # 0x44AE8
MIN_SIZE = HEADER2_OFF  # a save must at least hold header 1 + block 1

# Sizes of the two samples this package is verified against.
PS3_SAMPLE_SIZE = 0x4CAE8  # 314088
PSP_SAMPLE_SIZE = 0x3D9D0  # 252368

# --- fields inside the decrypted save ---------------------------------------

# The COMPACT layout drops 0x10 bytes from an array near offset 0xC0, so every
# field past that point sits 0x10 lower than in a STANDARD save.  Field offsets
# below are STANDARD; run them through `field_shift` for the actual save.
FIELD_SHIFT_FROM = 0xC8


def field_shift(region1):
    """How far fields move for a layout, relative to the standard one."""
    return region1 - REGION1_STANDARD


# Player name, 16 bytes.  Verified at the same place in six saves across both
# platforms and all three regions.  A second copy sits 0x20 later, filled in
# only on some saves.
PLAYER_NAME_OFF = 0x184
PLAYER_NAME_LEN = 16
PLAYER_NAME_ALT_OFF = 0x1A4

# The player's own record, laid out like a staff record but with the name at
# the front rather than at +0x20.
PLAYER_RECORD_OFF = 0x1CB88

# --- Mother Base staff roster ----------------------------------------------
#
# 350 records of 0xA0 bytes, which is Peace Walker's staff cap; the US and EU
# saves here are both full.  Story characters land at the front (MILLER,
# AMANDA, CHICO, HUEY), recruits get animal code names.
STAFF_OFF = 0x1CC28
STAFF_STRIDE = 0xA0
STAFF_MAX = 350

STAFF_ID_OFF = 0x00  # 0x20 bytes; per-soldier id, shared prefix between recruits
STAFF_ID_LEN = 0x20
STAFF_NAME_OFF = 0x20
STAFF_NAME_LEN = 0x10

# Eight combat sub-stats.  A save advertised as "all soldiers hacked with max
# stats" holds 1250 in all 2800 of these and nothing else, which is what pins
# both the offset and the cap.  `transfarmer` names them shoot, reload, throw,
# place, walk speed, run speed, fight, defense, in that order.
STAFF_COMBAT_OFF = 0x52
STAFF_COMBAT_COUNT = 8
STAFF_COMBAT_MAX = 1250

# Four department stat pairs.  In the same maxed save the first entry of each
# pair is 999 for every soldier.  `transfarmer` labels the four departments
# Mess Hall, Medical, R&D and Intel and each pair (min, max); the order and
# which half is which are not confirmed here.
STAFF_DEPT_OFFS = (0x64, 0x68, 0x6C, 0x70)
STAFF_DEPT_MAX = 999

# Small enumerated value, 0..15 across every sample. Likely the team or
# assignment; not confirmed.
STAFF_TEAM_OFF = 0x62

# --- Outer Ops unit table ---------------------------------------------------
#
# Same 0xA0 record size as the staff roster but a different field layout; holds
# captured vehicles (T-72U, MI-24A, ...).  48 entries in the PS3 sample, empty
# in a save that has not played Outer Ops.
OUTER_OPS_OFF = 0x103B0
OUTER_OPS_STRIDE = 0xA0
OUTER_OPS_MAX = 400

OUTER_OPS_NAME_OFF = 0x00
OUTER_OPS_NAME_LEN = 0x10
OUTER_OPS_BASE_GMP_OFF = 0x40
OUTER_OPS_STAT_PAIRS = (0x44, 0x48, 0x4C, 0x50)

# The table carries no entry count, so its end is found by shape: a record
# counts as a unit when it has a printable name and its numbers fall in the
# ranges every entry of the PS3 sample obeys.
OUTER_OPS_MAX_GMP = 20000
OUTER_OPS_MAX_STAT = 2000

# --- PS3/PSP title IDs that ship Peace Walker -------------------------------

TITLE_IDS = (
    "BLES01111",  # PS3, Europe
    "BLJM60228",  # PS3, Japan
    "BLUS30428",  # PS3, US
    "NPEB00678",  # PS3, Europe (digital)
    "NPED00686",  # PS3, Europe (digital)
    "NPJB40002",  # PS3, Japan (digital)
    "NPUB30611",  # PS3, US (digital)
    "ULES01372",  # PSP, Europe
    "ULJM05630",  # PSP, Japan
    "ULUS10509",  # PSP, US
    "NPJH50045",  # PSP, Japan (digital)
)
