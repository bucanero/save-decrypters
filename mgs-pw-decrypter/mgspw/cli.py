"""Command line front end."""

from __future__ import print_function

import argparse
import os
import re
import sys

from . import convert as convert_mod
from .crypto import SaveError
from .layout import PS3, PSP, STAFF_COMBAT_COUNT, TITLE_IDS
from .save import SCALARS, Save

VERSION = "0.1.0"

# Peace Walker names its save data files 00000000.000, 00000001.000, ...
SAVE_FILE_RE = re.compile(r"^\d{8}\.\d{3}$", re.IGNORECASE)


def _die(message):
    sys.stderr.write("error: %s\n" % message)
    return 1


def _report_checksums(bad):
    if bad is None:
        return
    if bad:
        for off in bad:
            print("  [!] checksum mismatch for the range at %#x" % off)
    else:
        print("  checksums OK")


def _load(path, encrypted=True):
    save, bad = Save.load(path, encrypted=encrypted)
    print(
        "%s: %s save, %d bytes" % (os.path.basename(path), save.platform, len(save.data))
    )
    _report_checksums(bad)
    return save


def _backup(path):
    target = path + ".bak"
    if not os.path.exists(target):
        with open(path, "rb") as src, open(target, "wb") as dst:
            dst.write(src.read())
        print("  backup written to %s" % target)


# --- commands ---------------------------------------------------------------


def cmd_info(args):
    save = _load(args.file, encrypted=not args.decrypted)
    if args.decrypted:
        _report_checksums(save.verify())
    print("  layout: %s" % save.layout_name)
    print("  player name: %r" % save.player_name)
    if save.player_name_alt:
        print("  second name copy: %r" % save.player_name_alt)
    staff = save.staff()
    print("  staff on Mother Base: %d" % len(staff))
    for entry in staff[:5]:
        print(
            "    [%3d] %-14r combat %s depts %s"
            % (entry.index, entry.name, entry.combat[:3] + ["..."], entry.departments)
        )
    if len(staff) > 5:
        print("    ... %d more" % (len(staff) - 5))
    units = save.outer_ops()
    print("  Outer Ops units: %d" % len(units))
    for entry in units[:3]:
        print(
            "    [%3d] %-14r base GMP %-6d %s"
            % (entry.index, entry.name, entry.base_gmp, entry.stats)
        )
    return 0


def cmd_decrypt(args):
    save = _load(args.file, encrypted=True)
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=False)
    print("  decrypted -> %s" % out)
    return 0


def cmd_encrypt(args):
    save = _load(args.file, encrypted=False)
    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=True)
    print("  checksums refreshed, encrypted -> %s" % out)
    return 0


def cmd_fix(args):
    save = _load(args.file, encrypted=False)
    before = save.verify()
    save.fix_checksums()
    out = args.output or args.file
    save.write(out, encrypted=False)
    print("  fixed %d checksum(s) -> %s" % (len(before), out))
    return 0


def cmd_name(args):
    save = _load(args.file, encrypted=not args.decrypted)
    if args.set is None:
        print("  player name: %r" % save.player_name)
        return 0
    save.player_name = args.set
    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=not args.decrypted)
    print("  player name set to %r -> %s" % (save.player_name, out))
    return 0


def cmd_staff(args):
    save = _load(args.file, encrypted=not args.decrypted)
    entries = save.staff(used_only=not args.all)
    if not entries:
        print("  no staff on Mother Base")
        return 0
    print("  idx  offset    name              team  combat (8)                    departments (4 pairs)")
    for entry in entries:
        print(
            "  %3d  %#08x  %-16r %-5d %-29s %s"
            % (
                entry.index,
                entry.offset,
                entry.name,
                entry.team,
                entry.combat,
                entry.departments,
            )
        )
    print("  %d staff" % len(entries))
    return 0


def cmd_staff_set(args):
    save = _load(args.file, encrypted=not args.decrypted)
    entries = {e.index: e for e in save.staff(used_only=False)}
    if args.index not in entries:
        return _die("staff index %d is out of range" % args.index)
    entry = entries[args.index]

    if args.name is not None:
        entry.name = args.name
    if args.team is not None:
        entry.team = args.team
    if args.combat is not None:
        if len(args.combat) == 1:
            entry.combat = args.combat * STAFF_COMBAT_COUNT
        elif len(args.combat) == STAFF_COMBAT_COUNT:
            entry.combat = args.combat
        else:
            return _die(
                "--combat takes 1 or %d comma-separated values" % STAFF_COMBAT_COUNT
            )
    for i in range(4):
        pair = getattr(args, "dept%d" % i)
        if pair is not None:
            entry.set_department(i, pair[0], pair[1])
    if args.max:
        entry.maximise()

    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=not args.decrypted)
    print(
        "  [%3d] %-14r team %d combat %s depts %s -> %s"
        % (entry.index, entry.name, entry.team, entry.combat, entry.departments, out)
    )
    return 0


def cmd_staff_max(args):
    save = _load(args.file, encrypted=not args.decrypted)
    entries = save.staff()
    for entry in entries:
        entry.maximise()
    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=not args.decrypted)
    print("  maxed the confirmed stats of %d staff -> %s" % (len(entries), out))
    return 0


def cmd_outer_ops(args):
    save = _load(args.file, encrypted=not args.decrypted)
    entries = save.outer_ops(used_only=not args.all)
    if not entries:
        print("  no Outer Ops units")
        return 0
    print("  idx  offset    name             base GMP  stat pairs")
    for entry in entries:
        print(
            "  %3d  %#08x  %-16r %-9d %s"
            % (entry.index, entry.offset, entry.name, entry.base_gmp, entry.stats)
        )
    return 0


def cmd_outer_ops_set(args):
    save = _load(args.file, encrypted=not args.decrypted)
    entries = {e.index: e for e in save.outer_ops(used_only=False)}
    if args.index not in entries:
        return _die("Outer Ops index %d is out of range" % args.index)
    entry = entries[args.index]

    if args.name is not None:
        entry.name = args.name
    if args.base_gmp is not None:
        entry.base_gmp = args.base_gmp
    for pair in range(4):
        value = getattr(args, "stat%d" % pair)
        if value is not None:
            entry.set_stat(pair, value[0], value[1])

    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=not args.decrypted)
    print(
        "  [%3d] %-14r base GMP %-6d %s -> %s"
        % (entry.index, entry.name, entry.base_gmp, entry.stats, out)
    )
    return 0


def cmd_peek(args):
    save = _load(args.file, encrypted=not args.decrypted)
    values = save.peek(args.offset, args.type, args.count)
    for i, value in enumerate(values):
        step = SCALARS[args.type][1]
        print("  %#08x  %s  %d (%#x)" % (args.offset + i * step, args.type, value, value))
    return 0


def cmd_poke(args):
    save = _load(args.file, encrypted=not args.decrypted)
    before = save.peek(args.offset, args.type)[0]
    save.poke(args.offset, args.type, args.value)
    save.fix_checksums()
    out = args.output or args.file
    if out == args.file and not args.no_backup:
        _backup(args.file)
    save.write(out, encrypted=not args.decrypted)
    print("  %#08x %s: %d -> %d, written to %s" % (args.offset, args.type, before, args.value, out))
    return 0


def cmd_convert(args):
    if args.direction == "psp2ps3":
        src_path, dst_path, src_want = args.psp, args.ps3, PSP
    else:
        src_path, dst_path, src_want = args.ps3, args.psp, PS3

    src = _load(src_path, encrypted=True)
    dst = _load(dst_path, encrypted=True)
    if src.platform != src_want:
        return _die(
            "%s is a %s save, expected %s" % (src_path, src.platform, src_want)
        )

    print("  source player name: %r" % src.player_name)
    if args.direction == "psp2ps3":
        mapping = convert_mod.psp_to_ps3(src, dst, args.with_block2)
    else:
        mapping = convert_mod.ps3_to_psp(src, dst, args.with_block2)

    print(
        "  mapping: main block %d words (%#x bytes)"
        % (mapping.copy_b1, mapping.copy_b1 * 4)
    )
    if mapping.remaining:
        print(
            "  %d words past the main block: %s"
            % (
                mapping.remaining,
                "copied to the second block" if mapping.copy_b2 else "left untouched",
            )
        )
    print("  result player name: %r" % dst.player_name)
    _report_checksums(dst.verify())

    out = args.output or dst_path
    if out == dst_path and not args.no_backup:
        _backup(dst_path)
    dst.write(out, encrypted=True)
    print("  %s -> %s" % (args.direction, out))
    return 0


def cmd_find(args):
    root = args.path
    if not os.path.isdir(root):
        return _die("%s is not a directory" % root)
    found = 0
    for dirpath, _dirnames, filenames in os.walk(root):
        base = os.path.basename(dirpath).upper()
        if not any(tid in base for tid in TITLE_IDS):
            continue
        for name in sorted(filenames):
            if not SAVE_FILE_RE.match(name):
                continue
            path = os.path.join(dirpath, name)
            size = os.path.getsize(path)
            try:
                save, bad = Save.load(path, encrypted=True)
            except (SaveError, ValueError) as exc:
                print("  %s  (%d bytes) -- %s" % (path, size, exc))
                continue
            found += 1
            print(
                "  %s\n      %s, %d bytes, player %r, checksums %s"
                % (
                    path,
                    "%s/%s" % (save.platform, save.layout_name),
                    size,
                    save.player_name,
                    "OK" if not bad else "BAD",
                )
            )
    if not found:
        print("  no Peace Walker saves found under %s" % root)
    return 0


# --- argument parsing -------------------------------------------------------


def _add_io(parser, backup=True):
    parser.add_argument("file", help="save file")
    parser.add_argument(
        "-d",
        "--decrypted",
        action="store_true",
        help="the file is already decrypted (default: treat it as encrypted)",
    )
    parser.add_argument("-o", "--output", help="write here instead of in place")
    if backup:
        parser.add_argument(
            "--no-backup",
            action="store_true",
            help="do not write a .bak next to an in-place edit",
        )


def _ints(text):
    return [int(part, 0) for part in text.split(",")]


def _pair(text):
    parts = text.split(",")
    if len(parts) != 2:
        raise argparse.ArgumentTypeError("expected MIN,MAX")
    return (int(parts[0], 0), int(parts[1], 0))


def build_parser():
    parser = argparse.ArgumentParser(
        prog="mgspw",
        description="Metal Gear Solid: Peace Walker save converter and editor "
        "(PS3 HD Edition and PSP).",
    )
    parser.add_argument("--version", action="version", version="mgspw " + VERSION)
    subs = parser.add_subparsers(dest="command")

    p = subs.add_parser("info", help="show platform, checksums, player, roster")
    _add_io(p, backup=False)
    p.set_defaults(func=cmd_info)

    p = subs.add_parser("decrypt", help="decrypt a save")
    p.add_argument("file")
    p.add_argument("-o", "--output")
    p.add_argument("--no-backup", action="store_true")
    p.set_defaults(func=cmd_decrypt)

    p = subs.add_parser("encrypt", help="refresh checksums and encrypt a save")
    p.add_argument("file")
    p.add_argument("-o", "--output")
    p.add_argument("--no-backup", action="store_true")
    p.set_defaults(func=cmd_encrypt)

    p = subs.add_parser("fix", help="recompute checksums of a decrypted save")
    p.add_argument("file")
    p.add_argument("-o", "--output")
    p.set_defaults(func=cmd_fix)

    p = subs.add_parser("name", help="read or set the player name")
    _add_io(p)
    p.add_argument("--set", help="new player name")
    p.set_defaults(func=cmd_name)

    p = subs.add_parser("staff", help="list the Mother Base staff roster")
    _add_io(p, backup=False)
    p.add_argument("--all", action="store_true", help="include empty slots")
    p.set_defaults(func=cmd_staff)

    p = subs.add_parser("staff-set", help="edit one staff member")
    _add_io(p)
    p.add_argument("index", type=int, help="staff index, as shown by `staff`")
    p.add_argument("--name")
    p.add_argument("--team", type=lambda v: int(v, 0))
    p.add_argument(
        "--combat",
        type=_ints,
        metavar="V or V,V,V,V,V,V,V,V",
        help="one value for all eight combat sub-stats, or all eight",
    )
    for i in range(4):
        p.add_argument(
            "--dept%d" % i,
            type=_pair,
            metavar="A,B",
            help="department pair %d at record offset %#x"
            % (i, (0x64, 0x68, 0x6C, 0x70)[i]),
        )
    p.add_argument(
        "--max", action="store_true", help="set every confirmed stat to its cap"
    )
    p.set_defaults(func=cmd_staff_set)

    p = subs.add_parser(
        "staff-max", help="set every staff member's confirmed stats to their cap"
    )
    _add_io(p)
    p.set_defaults(func=cmd_staff_max)

    p = subs.add_parser("outer-ops", help="list the Outer Ops unit table")
    _add_io(p, backup=False)
    p.add_argument("--all", action="store_true", help="include unused slots")
    p.set_defaults(func=cmd_outer_ops)

    p = subs.add_parser("outer-ops-set", help="edit one Outer Ops unit")
    _add_io(p)
    p.add_argument("index", type=int, help="unit index, as shown by `outer-ops`")
    p.add_argument("--name")
    p.add_argument("--base-gmp", type=lambda v: int(v, 0))
    for pair in range(4):
        p.add_argument(
            "--stat%d" % pair,
            type=_pair,
            metavar="A,B",
            help="stat pair %d at record offset %#x"
            % (pair, (0x44, 0x48, 0x4C, 0x50)[pair]),
        )
    p.set_defaults(func=cmd_outer_ops_set)

    p = subs.add_parser("peek", help="read raw values out of a save")
    _add_io(p, backup=False)
    p.add_argument("offset", type=lambda v: int(v, 0))
    p.add_argument("type", choices=sorted(SCALARS))
    p.add_argument("-n", "--count", type=int, default=1)
    p.set_defaults(func=cmd_peek)

    p = subs.add_parser("poke", help="write a raw value into a save")
    _add_io(p)
    p.add_argument("offset", type=lambda v: int(v, 0))
    p.add_argument("type", choices=sorted(SCALARS))
    p.add_argument("value", type=lambda v: int(v, 0))
    p.set_defaults(func=cmd_poke)

    p = subs.add_parser("convert", help="move save data between PSP and PS3")
    p.add_argument("direction", choices=("psp2ps3", "ps32psp"))
    p.add_argument("--psp", required=True, help="PSP save (encrypted)")
    p.add_argument("--ps3", required=True, help="PS3 save (encrypted)")
    p.add_argument("-o", "--output", help="write here instead of over the target")
    p.add_argument("--no-backup", action="store_true")
    p.add_argument(
        "--with-block2",
        action="store_true",
        help="also map the bytes past the main block onto the PS3 second "
        "block, as `transfarmer` does (off by default, see README)",
    )
    p.set_defaults(func=cmd_convert)

    p = subs.add_parser("find", help="list Peace Walker saves under a directory")
    p.add_argument("path", help="an RPCS3 install, a PSP drive, or any folder")
    p.set_defaults(func=cmd_find)

    return parser


def main(argv=None):
    parser = build_parser()
    args = parser.parse_args(argv)
    if not getattr(args, "func", None):
        parser.print_help()
        return 1
    try:
        return args.func(args)
    except (SaveError, ValueError, KeyError) as exc:
        return _die(str(exc))
    except IOError as exc:
        return _die(str(exc))
