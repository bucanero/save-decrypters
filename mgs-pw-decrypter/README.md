# mgs-pw-decrypter

A tool to decrypt Metal Gear Solid: Peace Walker save-games (PS3 HD Edition and PSP)

The save type is auto-detected, so the same command works for both platforms:

- **PS3 (HD Edition)**: two encrypted blocks, the main save data plus a second
  block holding the online/comrade data.
- **PSP**: only the main save data block, the second block doesn't exist.

The decrypted header is left in the save's native byte order (big-endian for
PS3, little-endian for PSP), so the checksums stored in it read as-is.

Note: this tool also updates the custom integrity checksums.

```
USAGE: ./mgs-pw-decrypter [option] filename

OPTIONS        Explanation:
 -d            Decrypt File
 -e            Encrypt File
```

### Save layouts

Releases don't all use the same offsets. An array near `0xC0` is four `u32`
shorter in some builds, which takes `0x10` bytes off the first checksummed
region and shifts every later boundary down to match. The layout is detected
from the save size and reported on every run:

| Layout | First region | Main block | PSP save size | Seen in |
| :--- | :--- | :--- | :--- | :--- |
| standard | `0x1AF24` | `0x35998` | `0x3D9D0` | PSP US/EU, every PS3 save |
| compact | `0x1AF14` | `0x35988` | `0x3D9C0` | PSP JP digital (NPJH50045) |

### PSP saves

A PSP `00000000.000` copied straight off a Memory Stick has a second, outer
layer on top of this one: the PSP savedata encryption (`sceChnnlsv`/KIRK),
keyed by a per-game secure key that has to be dumped from the console. Strip
that layer first and feed the result to this tool. Anything that implements it
will do -- [Apollo Save Tool](https://github.com/bucanero/apollo-psp) does it
on the PSP itself, and its `source/psp_decrypter.c` is the reference
implementation.

Saves written by emulators with savedata encryption turned off have no outer
layer and can be used directly.

### Samples

`samples/` holds one encrypted/decrypted pair per layout, so a change can be
checked against all of them:

| File | Size | Save |
| :--- | :--- | :--- |
| `00000000.000` / `.dec` | `0x4CAE8` | PS3 HD Edition, standard layout |
| `00000000.000.PSP.enc` / `.PSP.dec` | `0x3D9D0` | PSP US/EU, standard layout |
| `00000000.000.PSP-JP.enc` / `.PSP-JP.dec` | `0x3D9C0` | PSP JP digital (NPJH50045), compact layout |

The PSP files are game-encrypted only -- the outer PSP savedata layer has
already been removed, which is the form this tool expects.

`mgs-pw.cs` and `mgs-peace-walker-secfixer-v1000.rar` are the original Xbox 360
SecFixer this tool was reversed from.

---

## mgspw.py -- reference Python tool

Alongside the C decrypter there is a Python 3 implementation that does the same
crypto plus a few extras: moving a save between the PSP and PS3 releases, and
editing the fields that have been mapped. It needs no installation and no
dependencies, and it produces **byte-identical** output to the C tool on both
layouts, so the two are interchangeable.

This is a reference and advanced-user tool. For everyday decrypting, use the C
tool above.

```bash
python3 mgspw.py --help
```

```
info            show platform, layout, checksums, player, staff, Outer Ops
decrypt         decrypt a save
encrypt         refresh checksums and encrypt a save
fix             recompute the checksums of a decrypted save
name            read or set the player name
staff           list the Mother Base staff roster
staff-set       edit one staff member
staff-max       set every staff member's confirmed stats to their cap
outer-ops       list the Outer Ops unit table
outer-ops-set   edit one Outer Ops unit
peek            read raw values out of a save
poke            write a raw value into a save
convert         move save data between PSP and PS3
find            list Peace Walker saves under a directory
```

Commands take an encrypted save by default and handle the whole
decrypt/edit/checksum/encrypt cycle. Pass `-d` to work on an already-decrypted
file and `-o` to write elsewhere; an in-place edit writes a `.bak` first unless
you pass `--no-backup`. Like the C tool, it expects the PSP savedata layer to
have been removed already.

### Converting between platforms

Both directions need a save from the target platform to write into. Only the
shared main block is carried over; the target save supplies everything else.

```bash
python3 mgspw.py convert psp2ps3 --psp ms0/00000000.000 --ps3 rpcs3/00000000.000
python3 mgspw.py convert ps32psp --ps3 rpcs3/00000000.000 --psp ms0/00000000.000
```

### Editing

```bash
python3 mgspw.py staff samples/00000000.000.PSP.enc
python3 mgspw.py staff-set save.000 0 --name PANTHER --combat 1250
python3 mgspw.py staff-max save.000        # cap every soldier's known stats
python3 mgspw.py name save.000 --set BIGBOSS
python3 mgspw.py peek save.000 0x1cc28 u16 -n 16
```

`peek`/`poke` take `u8 u16 u32 s8 s16 s32` and are the escape hatch for
anything not mapped yet. Every write refreshes the checksums.

### As a library

```python
from mgspw import Save

save, bad_checksums = Save.from_encrypted(open("00000000.000", "rb").read())
print(save.platform, save.layout_name, save.player_name, len(save.staff()))
for soldier in save.staff():
    soldier.maximise()
open("out.000", "wb").write(save.to_encrypted())   # checksums refreshed
```

### Save format

Both releases share one format. The PS3 save is the PSP save with a second,
PS3-only block appended, and the shared part is **little-endian on both
platforms** -- the HD port kept the PSP's structure wholesale.

| Region | Offset | Size | PSP | PS3 |
| :--- | :--- | :--- | :--- | :--- |
| Header 1 | `0x0` | `0x40` | yes | yes |
| Main block (encrypted, LE) | `0x40` | `0x35998` | yes | yes |
| Header 2 | `0x359D8` | `0x40` | -- | yes |
| Second block (encrypted, BE) | `0x35A18` | `0xF0D0` | -- | yes |
| Thumbnail | `0x44AE8` | `0x8000` | -- | yes |

Each encrypted block is XORed with a keystream from a linear congruential
generator (`s0 = s0 * 0x2E90EDD + s1`) seeded from the plain-text header in
front of it. Three checksums in header 1 cover the main block; the PS3 has a
fourth in header 2 for the second block. The platform is detected by testing
whether a valid seed header decodes at `0x359D8`, which works on encrypted and
decrypted files alike. Sizes above are the standard layout; see the table
earlier for the compact one.

### What is mapped

Verified against six saves -- PSP US, EU and two JP, plus the PS3 and PSP
samples here:

- **Player name** -- `0x184`, 16 bytes; the longest observed is 15 characters
  (`D3@DLY-P$YK0777`), so the field is used to its limit. A second copy sits at
  `0x1A4`, filled in on some saves only.
- **Mother Base staff roster** -- `0x1CC28`, `0xA0` per record, cap **350**
  (Peace Walker's own limit). Per record: a `0x20`-byte soldier id, a 16-byte
  code name at `+0x20`, eight combat sub-stats at `+0x52` capped at **1250**,
  and four department stat pairs at `+0x64`, `+0x68`, `+0x6C`, `+0x70` whose
  first entry caps at **999**. Story characters lead the list -- the EU save
  starts MILLER, AMANDA, CHICO, HUEY -- and recruits get animal code names.
- **The player's own record** -- `0x1CB88`, same shape but with the name at the
  front instead of `+0x20`.
- **Outer Ops units** -- `0x103B0`, `0xA0` per record: a 16-byte name at
  `+0x00`, a base GMP cost at `+0x40`, four pairs at `+0x44`..`+0x53`.

On a compact-layout save every field past `0xC8` sits `0x10` lower; the tool
applies that shift for you.

The combat and department caps are not guesses: a save advertised as "all
soldiers hacked with max stats" holds 1250 in all 2800 combat values and
nothing else, and 999 in the first half of every department pair.

**Not mapped**, and left out rather than guessed: which department each stat
pair is, whether the pairs are `(min, max)` (the ordering only holds for
65-99% of soldiers depending on the save, so probably not), and morale, life,
psyche and base GMP for staff. `+0x62` is a small enumerated value exposed as
`team` but unconfirmed. Use `peek` for those.

### Tests

```bash
cd mgs-pw-decrypter && python3 -m unittest discover -s tests -v
```

34 tests against the files in `samples/`: decryption byte-for-byte against the
C tool's committed output on all three pairs, round-trips, every checksum range
computed independently, both conversion directions, a PSP -> PS3 -> PSP round
trip that restores the original file exactly, and the compact layout's `0x10`
field shift.

### Credits

This tool is based (reversed) on the original XBOX `MGS Peace Walker - SecFixer` by Philymaster

The PSP/PS3 block mapping and the title ID list were cross-checked against
[Transfarmer](https://qberty.com/transfarmer-rpcs3-psp-save-converter-editor/)
by qberty, a Windows-only Rust tool that converts these saves.
