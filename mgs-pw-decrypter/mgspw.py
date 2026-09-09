#!/usr/bin/env python3
"""Convenience wrapper so the tool can be run as ./mgspw.py without installing."""

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from mgspw.cli import main  # noqa: E402

if __name__ == "__main__":
    sys.exit(main())
