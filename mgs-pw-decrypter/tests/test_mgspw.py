"""Verification against the real saves in ../samples.

Run with:  python3 -m unittest discover -s tests -v
"""

import os
import sys
import unittest

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))

from mgspw import PS3, PSP, Save, crypto, layout  # noqa: E402
from mgspw import convert as convert_mod  # noqa: E402

SAMPLES = os.path.join(os.path.dirname(HERE), "samples")
PS3_ENC = os.path.join(SAMPLES, "00000000.000")
PS3_DEC = os.path.join(SAMPLES, "00000000.000.dec")
PSP_ENC = os.path.join(SAMPLES, "00000000.000.PSP.enc")
PSP_DEC = os.path.join(SAMPLES, "00000000.000.PSP.dec")
# A PSP JP digital save, which uses the shorter COMPACT layout.
JP_ENC = os.path.join(SAMPLES, "00000000.000.PSP-JP.enc")
JP_DEC = os.path.join(SAMPLES, "00000000.000.PSP-JP.dec")


def read(path):
    with open(path, "rb") as handle:
        return handle.read()


@unittest.skipUnless(os.path.isdir(SAMPLES), "sample saves not available")
class TestCrypto(unittest.TestCase):
    def test_detect(self):
        self.assertEqual(crypto.detect(bytearray(read(PS3_ENC))), PS3)
        self.assertEqual(crypto.detect(bytearray(read(PSP_ENC))), PSP)

    def test_detect_works_on_decrypted_files_too(self):
        self.assertEqual(crypto.detect(bytearray(read(PS3_DEC))), PS3)
        self.assertEqual(crypto.detect(bytearray(read(PSP_DEC))), PSP)

    def test_detect_rejects_short_files(self):
        with self.assertRaises(crypto.SaveError):
            crypto.detect(bytearray(b"nope"))

    def test_ps3_decrypt_matches_reference(self):
        """Byte-for-byte against the committed output of mgs-pw-decrypter."""
        buf = bytearray(read(PS3_ENC))
        platform, bad = crypto.decrypt(buf)
        self.assertEqual(platform, PS3)
        self.assertEqual(bad, [])
        self.assertEqual(bytes(buf), read(PS3_DEC))

    def test_psp_decrypt_matches_reference(self):
        buf = bytearray(read(PSP_ENC))
        platform, bad = crypto.decrypt(buf)
        self.assertEqual(platform, PSP)
        self.assertEqual(bad, [])
        self.assertEqual(bytes(buf), read(PSP_DEC))

    def test_ps3_roundtrip(self):
        buf = bytearray(read(PS3_ENC))
        crypto.decrypt(buf)
        crypto.encrypt(buf)
        self.assertEqual(bytes(buf), read(PS3_ENC))

    def test_psp_roundtrip(self):
        buf = bytearray(read(PSP_ENC))
        crypto.decrypt(buf)
        crypto.encrypt(buf)
        self.assertEqual(bytes(buf), read(PSP_ENC))

    def test_psp_encryption_only_touches_the_main_block(self):
        enc = read(PSP_ENC)
        dec = read(PSP_DEC)
        differing = [i for i in range(len(enc)) if enc[i] != dec[i]]
        self.assertEqual(differing[0], layout.BLOCK1_OFF)
        self.assertEqual(
            differing[-1] + 1, layout.BLOCK1_OFF + layout.BLOCK1_SIZE
        )

    def test_checksum_ranges(self):
        """Every stored checksum matches, computed independently per range."""
        for path, platform in ((PS3_ENC, PS3), (PSP_ENC, PSP)):
            buf = bytearray(read(path))
            crypto._xor_block(
                buf,
                layout.BLOCK1_OFF,
                layout.BLOCK1_WORDS,
                crypto._salts(buf, 0, crypto.LE),
                crypto.LE,
            )
            for index, off, length in layout.CHECKSUMS:
                got = crypto.checksum(buf[off : off + length])
                want = int.from_bytes(buf[index * 4 : index * 4 + 4], "little")
                self.assertEqual(got, want, "%s range %#x" % (platform, off))


@unittest.skipUnless(os.path.isdir(SAMPLES), "sample saves not available")
class TestSave(unittest.TestCase):
    def test_player_name(self):
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        self.assertEqual(ps3.player_name, "Z3RO")
        self.assertEqual(psp.player_name, "WICK")

    def test_verify_on_a_loaded_save(self):
        for path in (PS3_ENC, PSP_ENC):
            save, bad = Save.from_encrypted(read(path))
            self.assertEqual(bad, [])
            self.assertEqual(save.verify(), [])

    def test_edit_then_reencrypt_keeps_checksums_valid(self):
        for path in (PS3_ENC, PSP_ENC):
            save, _ = Save.from_encrypted(read(path))
            save.poke(0x1000, "u32", 0xDEADBEEF)
            self.assertNotEqual(save.verify(), [])
            reloaded, bad = Save.from_encrypted(save.to_encrypted())
            self.assertEqual(bad, [], path)
            self.assertEqual(reloaded.peek(0x1000, "u32"), [0xDEADBEEF])

    def test_set_player_name(self):
        save, _ = Save.from_encrypted(read(PSP_ENC))
        save.player_name = "BOSS"
        reloaded, bad = Save.from_encrypted(save.to_encrypted())
        self.assertEqual(bad, [])
        self.assertEqual(reloaded.player_name, "BOSS")

    def test_name_length_is_checked(self):
        save, _ = Save.from_encrypted(read(PSP_ENC))
        with self.assertRaises(ValueError):
            save.player_name = "X" * 16

    def test_outer_ops(self):
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        entries = ps3.outer_ops()
        self.assertEqual(len(entries), 48)
        self.assertEqual(entries[0].offset, layout.OUTER_OPS_OFF)
        self.assertEqual(entries[0].name, "T-72U")
        self.assertEqual(entries[0].base_gmp, 1850)
        self.assertEqual(entries[0].stats[3], (80, 100))
        self.assertEqual(entries[-1].name, "KPZ 70(M)")
        # the PSP sample has captured no units
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        self.assertEqual(psp.outer_ops(), [])

    def test_outer_ops_edit(self):
        save, _ = Save.from_encrypted(read(PS3_ENC))
        entry = save.outer_ops()[0]
        entry.name = "TESTUNIT"
        entry.base_gmp = 1234
        entry.set_stat(0, 11, 22)
        reloaded, bad = Save.from_encrypted(save.to_encrypted())
        self.assertEqual(bad, [])
        again = reloaded.outer_ops()[0]
        self.assertEqual(again.name, "TESTUNIT")
        self.assertEqual(again.base_gmp, 1234)
        self.assertEqual(again.stats[0], (11, 22))


@unittest.skipUnless(os.path.isdir(SAMPLES), "sample saves not available")
class TestConvert(unittest.TestCase):
    def test_plan_matches_transfarmer(self):
        """Word counts derived from transfarmer's own mapping arithmetic."""
        mapping = convert_mod.plan(layout.PSP_SAMPLE_SIZE, layout.PS3_SAMPLE_SIZE)
        self.assertEqual(mapping.psp_game_words, layout.PSP_SAMPLE_SIZE // 4 - 16)
        self.assertEqual(mapping.copy_b1, layout.BLOCK1_WORDS)
        self.assertEqual(mapping.remaining, 0x1FFE)
        self.assertEqual(mapping.copy_b2, 0)

    def test_plan_with_block2(self):
        mapping = convert_mod.plan(
            layout.PSP_SAMPLE_SIZE, layout.PS3_SAMPLE_SIZE, include_block2=True
        )
        self.assertEqual(mapping.copy_b2, 0x1FFE)

    def test_psp_to_ps3_carries_the_save_over(self):
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        convert_mod.psp_to_ps3(psp, ps3)

        self.assertEqual(ps3.player_name, "WICK")
        self.assertEqual(ps3.verify(), [])
        # the whole main block now matches the PSP source
        self.assertEqual(
            ps3.data[layout.BLOCK1_OFF : layout.BLOCK1_OFF + layout.BLOCK1_SIZE],
            psp.data[layout.BLOCK1_OFF : layout.BLOCK1_OFF + layout.BLOCK1_SIZE],
        )
        # the result is a valid PS3 save that reloads cleanly
        reloaded, bad = Save.from_encrypted(ps3.to_encrypted())
        self.assertEqual(bad, [])
        self.assertEqual(reloaded.platform, PS3)
        self.assertEqual(reloaded.player_name, "WICK")

    def test_ps3_to_psp_carries_the_save_over(self):
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        convert_mod.ps3_to_psp(ps3, psp)

        self.assertEqual(psp.player_name, "Z3RO")
        self.assertEqual(psp.verify(), [])
        self.assertEqual(len(psp.outer_ops()), 48)
        reloaded, bad = Save.from_encrypted(psp.to_encrypted())
        self.assertEqual(bad, [])
        self.assertEqual(reloaded.platform, PSP)
        self.assertEqual(reloaded.player_name, "Z3RO")
        self.assertEqual(reloaded.outer_ops()[0].name, "T-72U")

    def test_ps3_to_psp_leaves_the_opaque_region_alone(self):
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        before = bytes(psp.data[layout.HEADER2_OFF :])
        convert_mod.ps3_to_psp(ps3, psp)
        self.assertEqual(bytes(psp.data[layout.HEADER2_OFF :]), before)

    def test_round_trip_through_both_conversions(self):
        """PSP -> PS3 -> PSP restores the original main block exactly."""
        psp, _ = Save.from_encrypted(read(PSP_ENC))
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        original = bytes(
            psp.data[layout.BLOCK1_OFF : layout.BLOCK1_OFF + layout.BLOCK1_SIZE]
        )

        convert_mod.psp_to_ps3(psp, ps3)
        back, _ = Save.from_encrypted(read(PSP_ENC))
        convert_mod.ps3_to_psp(ps3, back)

        self.assertEqual(
            bytes(back.data[layout.BLOCK1_OFF : layout.BLOCK1_OFF + layout.BLOCK1_SIZE]),
            original,
        )
        self.assertEqual(back.to_encrypted(), read(PSP_ENC))

    def test_wrong_platform_is_rejected(self):
        ps3a, _ = Save.from_encrypted(read(PS3_ENC))
        ps3b, _ = Save.from_encrypted(read(PS3_ENC))
        with self.assertRaises(crypto.SaveError):
            convert_mod.psp_to_ps3(ps3a, ps3b)


@unittest.skipUnless(os.path.isdir(SAMPLES), "sample saves not available")
class TestSharedFormat(unittest.TestCase):
    def test_main_block_is_little_endian_on_both(self):
        """A field that must be small reads sanely as LE on both platforms."""
        ps3, _ = Save.from_encrypted(read(PS3_ENC))
        entry = ps3.outer_ops()[0]
        # base GMP and the stat pairs are only plausible read little-endian
        self.assertLess(entry.base_gmp, 20000)
        for low, high in entry.stats:
            self.assertLessEqual(low, high)
            self.assertLessEqual(high, 2000)


if __name__ == "__main__":
    unittest.main()


@unittest.skipUnless(os.path.exists(JP_ENC), "JP sample not available")
class TestCompactSample(unittest.TestCase):
    """The COMPACT layout, covered by a committed sample."""

    def test_detected_as_compact(self):
        buf = bytearray(read(JP_ENC))
        self.assertEqual(crypto.detect(buf), PSP)
        self.assertEqual(crypto.layout_for(buf), layout.REGION1_COMPACT)
        self.assertEqual(len(buf), layout.psp_size(layout.REGION1_COMPACT))

    def test_decrypt_matches_reference(self):
        buf = bytearray(read(JP_ENC))
        platform, bad = crypto.decrypt(buf)
        self.assertEqual(platform, PSP)
        self.assertEqual(bad, [], "COMPACT checksums")
        self.assertEqual(bytes(buf), read(JP_DEC))

    def test_roundtrip(self):
        buf = bytearray(read(JP_ENC))
        crypto.decrypt(buf)
        crypto.encrypt(buf)
        self.assertEqual(bytes(buf), read(JP_ENC))

    def test_fields_are_shifted(self):
        save, bad = Save.from_encrypted(read(JP_ENC))
        self.assertEqual(bad, [])
        self.assertTrue(save.compact)
        self.assertEqual(save.layout_name, "COMPACT")
        # every field past the divergence point moves down by 0x10
        self.assertEqual(save.field(layout.PLAYER_NAME_OFF), 0x174)
        self.assertEqual(save.field(layout.STAFF_OFF), 0x1CC18)
        self.assertEqual(save.field(layout.OUTER_OPS_OFF), 0x103A0)
        # and the early fields do not
        self.assertEqual(save.field(0x40), 0x40)

    def test_reads_the_shifted_data(self):
        """With standard offsets this save looks empty; with the shift it does not."""
        save, _ = Save.from_encrypted(read(JP_ENC))
        self.assertEqual(save.player_name, "DINESY")
        self.assertEqual(len(save.staff()), 321)
        self.assertEqual(save.staff()[0].name, "MAGPIE")

    def test_editing_a_compact_save(self):
        save, _ = Save.from_encrypted(read(JP_ENC))
        save.player_name = "SNAKE"
        save.staff()[0].name = "PANTHER"
        reloaded, bad = Save.from_encrypted(save.to_encrypted())
        self.assertEqual(bad, [], "checksums after editing a COMPACT save")
        self.assertEqual(reloaded.player_name, "SNAKE")
        self.assertEqual(reloaded.staff()[0].name, "PANTHER")

    def test_the_two_layouts_differ_by_one_block(self):
        std, _ = Save.from_encrypted(read(PSP_ENC))
        cmp_, _ = Save.from_encrypted(read(JP_ENC))
        self.assertEqual(len(std.data) - len(cmp_.data), 0x10)
        self.assertEqual(
            layout.block1_size(std.region1) - layout.block1_size(cmp_.region1), 0x10
        )
        # the trailer past the main block is the same in both
        for save in (std, cmp_):
            trailer = len(save.data) - 0x40 - layout.block1_size(save.region1)
            self.assertEqual(trailer, layout.PSP_TRAILER_SIZE)


class TestLayoutVariants(unittest.TestCase):
    def test_derived_layouts_match_the_measured_ones(self):
        """Both layouts reproduce the ranges found empirically in real saves."""
        self.assertEqual(
            layout.checksum_ranges(layout.REGION1_STANDARD),
            ((12, 0x1CB68, 0x18E68), (14, 0x44, 0x1AF24), (15, 0x1AF68, 0x1C00)),
        )
        self.assertEqual(
            layout.checksum_ranges(layout.REGION1_COMPACT),
            ((12, 0x1CB58, 0x18E68), (14, 0x44, 0x1AF14), (15, 0x1AF58, 0x1C00)),
        )
        self.assertEqual(layout.block1_size(layout.REGION1_STANDARD), 0x35998)
        self.assertEqual(layout.block1_size(layout.REGION1_COMPACT), 0x35988)
        self.assertEqual(layout.psp_size(layout.REGION1_STANDARD), 0x3D9D0)
        self.assertEqual(layout.psp_size(layout.REGION1_COMPACT), 0x3D9C0)

    def test_field_shift(self):
        self.assertEqual(layout.field_shift(layout.REGION1_STANDARD), 0)
        self.assertEqual(layout.field_shift(layout.REGION1_COMPACT), -0x10)

    def test_ps3_always_standard(self):
        buf = bytearray(read(PS3_ENC)) if os.path.exists(PS3_ENC) else None
        if buf is None:
            self.skipTest("sample not available")
        self.assertEqual(crypto.layout_for(buf), layout.REGION1_STANDARD)
