"""A loaded, decrypted Peace Walker save and the fields we can read from it."""

import struct

from . import crypto, layout
from .layout import (
    OUTER_OPS_BASE_GMP_OFF,
    OUTER_OPS_MAX,
    OUTER_OPS_MAX_GMP,
    OUTER_OPS_MAX_STAT,
    OUTER_OPS_NAME_LEN,
    OUTER_OPS_NAME_OFF,
    OUTER_OPS_OFF,
    OUTER_OPS_STAT_PAIRS,
    OUTER_OPS_STRIDE,
    PLAYER_NAME_ALT_OFF,
    PLAYER_NAME_LEN,
    PLAYER_NAME_OFF,
    PLAYER_RECORD_OFF,
    STAFF_COMBAT_COUNT,
    STAFF_COMBAT_MAX,
    STAFF_COMBAT_OFF,
    STAFF_DEPT_MAX,
    STAFF_DEPT_OFFS,
    STAFF_ID_LEN,
    STAFF_ID_OFF,
    STAFF_MAX,
    STAFF_NAME_LEN,
    STAFF_NAME_OFF,
    STAFF_OFF,
    STAFF_STRIDE,
    STAFF_TEAM_OFF,
)

SCALARS = {
    "u8": ("B", 1),
    "u16": ("H", 2),
    "u32": ("I", 4),
    "s8": ("b", 1),
    "s16": ("h", 2),
    "s32": ("i", 4),
}


def _read_name(buf, off, length):
    return bytes(buf[off : off + length]).split(b"\0")[0].decode("latin-1")


def _write_name(buf, off, length, value):
    encoded = value.encode("latin-1")
    if len(encoded) >= length:
        raise ValueError("name must be under %d bytes" % length)
    buf[off : off + length] = encoded + b"\0" * (length - len(encoded))


class _Record(object):
    """A fixed-size record inside a save."""

    __slots__ = ("_save", "index", "offset", "_stride")

    def __init__(self, save, index, base, stride):
        self._save = save
        self.index = index
        self.offset = base + index * stride
        self._stride = stride

    @property
    def raw(self):
        return bytes(self._save.data[self.offset : self.offset + self._stride])

    def _u16(self, rel):
        return struct.unpack_from("<H", self._save.data, self.offset + rel)[0]

    def _set_u16(self, rel, value):
        struct.pack_into("<H", self._save.data, self.offset + rel, value & 0xFFFF)


class StaffEntry(_Record):
    """One Mother Base staff member."""

    @property
    def used(self):
        start = self.offset + STAFF_NAME_OFF
        name = bytes(self._save.data[start : start + STAFF_NAME_LEN]).split(b"\0")[0]
        return bool(name) and all(0x20 <= b < 0x7F for b in name)

    @property
    def soldier_id(self):
        return self.raw[STAFF_ID_OFF : STAFF_ID_OFF + STAFF_ID_LEN]

    @property
    def name(self):
        return _read_name(self._save.data, self.offset + STAFF_NAME_OFF, STAFF_NAME_LEN)

    @name.setter
    def name(self, value):
        _write_name(self._save.data, self.offset + STAFF_NAME_OFF, STAFF_NAME_LEN, value)

    @property
    def team(self):
        """Small enumerated value; likely the team, not confirmed."""
        return self._u16(STAFF_TEAM_OFF)

    @team.setter
    def team(self, value):
        self._set_u16(STAFF_TEAM_OFF, value)

    @property
    def combat(self):
        """The eight combat sub-stats."""
        return list(
            struct.unpack_from(
                "<%dH" % STAFF_COMBAT_COUNT,
                self._save.data,
                self.offset + STAFF_COMBAT_OFF,
            )
        )

    @combat.setter
    def combat(self, values):
        if len(values) != STAFF_COMBAT_COUNT:
            raise ValueError("need %d combat values" % STAFF_COMBAT_COUNT)
        struct.pack_into(
            "<%dH" % STAFF_COMBAT_COUNT,
            self._save.data,
            self.offset + STAFF_COMBAT_OFF,
            *[v & 0xFFFF for v in values]
        )

    @property
    def departments(self):
        """The four department stat pairs, in table order."""
        return [(self._u16(o), self._u16(o + 2)) for o in STAFF_DEPT_OFFS]

    def set_department(self, index, first, second):
        rel = STAFF_DEPT_OFFS[index]
        self._set_u16(rel, first)
        self._set_u16(rel + 2, second)

    def maximise(self):
        """Set every stat we have confirmed a cap for to that cap."""
        self.combat = [STAFF_COMBAT_MAX] * STAFF_COMBAT_COUNT
        for i in range(len(STAFF_DEPT_OFFS)):
            self.set_department(i, STAFF_DEPT_MAX, STAFF_DEPT_MAX)

    def __repr__(self):
        return "<StaffEntry %d @%#x %r>" % (self.index, self.offset, self.name)


class OuterOpsEntry(_Record):
    """One captured Outer Ops unit."""

    @property
    def used(self):
        name = self.raw[OUTER_OPS_NAME_OFF : OUTER_OPS_NAME_OFF + OUTER_OPS_NAME_LEN]
        name = name.split(b"\0")[0]
        if not name or not all(0x20 <= b < 0x7F for b in name):
            return False
        if not 0 < self.base_gmp <= OUTER_OPS_MAX_GMP:
            return False
        for low, high in self.stats:
            if low > high or high > OUTER_OPS_MAX_STAT:
                return False
        return True

    @property
    def name(self):
        return _read_name(
            self._save.data, self.offset + OUTER_OPS_NAME_OFF, OUTER_OPS_NAME_LEN
        )

    @name.setter
    def name(self, value):
        _write_name(
            self._save.data,
            self.offset + OUTER_OPS_NAME_OFF,
            OUTER_OPS_NAME_LEN,
            value,
        )

    @property
    def base_gmp(self):
        return self._u16(OUTER_OPS_BASE_GMP_OFF)

    @base_gmp.setter
    def base_gmp(self, value):
        self._set_u16(OUTER_OPS_BASE_GMP_OFF, value)

    @property
    def stats(self):
        return [(self._u16(o), self._u16(o + 2)) for o in OUTER_OPS_STAT_PAIRS]

    def set_stat(self, index, low, high):
        rel = OUTER_OPS_STAT_PAIRS[index]
        self._set_u16(rel, low)
        self._set_u16(rel + 2, high)

    def __repr__(self):
        return "<OuterOpsEntry %d @%#x %r>" % (self.index, self.offset, self.name)


class Save(object):
    """A decrypted save, plus the platform and layout it came from."""

    def __init__(self, data, platform, region1=None):
        self.data = bytearray(data)
        self.platform = platform
        self.region1 = (
            crypto.layout_for(self.data, platform) if region1 is None else region1
        )

    # --- loading and saving -------------------------------------------------

    @classmethod
    def from_encrypted(cls, raw):
        """Decrypt `raw`. Returns (save, list of bad checksum offsets)."""
        buf = bytearray(raw)
        platform, bad = crypto.decrypt(buf)
        return cls(buf, platform), bad

    @classmethod
    def from_decrypted(cls, raw):
        buf = bytearray(raw)
        return cls(buf, crypto.detect(buf))

    @classmethod
    def load(cls, path, encrypted=True):
        with open(path, "rb") as handle:
            raw = handle.read()
        if encrypted:
            return cls.from_encrypted(raw)
        return cls.from_decrypted(raw), None

    def to_encrypted(self):
        buf = bytearray(self.data)
        crypto.encrypt(buf)
        return bytes(buf)

    def to_decrypted(self):
        return bytes(self.data)

    def write(self, path, encrypted=True):
        blob = self.to_encrypted() if encrypted else self.to_decrypted()
        with open(path, "wb") as handle:
            handle.write(blob)

    # --- layout -------------------------------------------------------------

    @property
    def compact(self):
        """Whether this save uses the shorter layout."""
        return self.region1 != layout.REGION1_STANDARD

    @property
    def layout_name(self):
        return "COMPACT" if self.compact else "STANDARD"

    def field(self, standard_offset):
        """Translate a standard-layout field offset for this save."""
        if standard_offset >= layout.FIELD_SHIFT_FROM:
            return standard_offset + layout.field_shift(self.region1)
        return standard_offset

    # --- integrity ----------------------------------------------------------

    @property
    def _hdr_endian(self):
        return crypto.header_endian(self.platform, decrypted=True)

    def verify(self):
        return crypto.verify(self.data, self.platform, self._hdr_endian, self.region1)

    def fix_checksums(self):
        crypto.write_checksums(
            self.data, self.platform, self._hdr_endian, self.region1
        )

    # --- fields -------------------------------------------------------------

    @property
    def player_name(self):
        return _read_name(self.data, self.field(PLAYER_NAME_OFF), PLAYER_NAME_LEN)

    @player_name.setter
    def player_name(self, value):
        _write_name(self.data, self.field(PLAYER_NAME_OFF), PLAYER_NAME_LEN, value)
        alt = self.field(PLAYER_NAME_ALT_OFF)
        # Only touch the second copy on saves that actually use it.
        if bytes(self.data[alt : alt + PLAYER_NAME_LEN]).split(b"\0")[0]:
            _write_name(self.data, alt, PLAYER_NAME_LEN, value)

    @property
    def player_name_alt(self):
        return _read_name(self.data, self.field(PLAYER_NAME_ALT_OFF), PLAYER_NAME_LEN)

    def _records(self, cls, base, stride, limit, used_only):
        out = []
        for index in range(limit):
            if base + (index + 1) * stride > len(self.data):
                break
            entry = cls(self, index, base, stride)
            if used_only and not entry.used:
                break
            out.append(entry)
        return out

    def staff(self, used_only=True, limit=STAFF_MAX):
        """The Mother Base staff roster."""
        return self._records(
            StaffEntry, self.field(STAFF_OFF), STAFF_STRIDE, limit, used_only
        )

    def player_record(self):
        """The player's own record, which sits just before the staff array."""
        return StaffEntry(self, 0, self.field(PLAYER_RECORD_OFF), STAFF_STRIDE)

    def outer_ops(self, used_only=True, limit=OUTER_OPS_MAX):
        """The captured Outer Ops units."""
        return self._records(
            OuterOpsEntry,
            self.field(OUTER_OPS_OFF),
            OUTER_OPS_STRIDE,
            limit,
            used_only,
        )

    # --- generic access -----------------------------------------------------

    def peek(self, offset, kind, count=1):
        code, size = SCALARS[kind]
        if offset + size * count > len(self.data):
            raise ValueError("read past end of save")
        return list(struct.unpack_from("<%d%s" % (count, code), self.data, offset))

    def poke(self, offset, kind, value):
        code, size = SCALARS[kind]
        if offset + size > len(self.data):
            raise ValueError("write past end of save")
        struct.pack_into("<" + code, self.data, offset, value)

    def __repr__(self):
        return "<Save %s/%s 0x%X bytes player=%r>" % (
            self.platform,
            self.layout_name,
            len(self.data),
            self.player_name,
        )
