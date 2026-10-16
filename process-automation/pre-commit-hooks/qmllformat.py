#!/usr/bin/env python3
"""Format QML files with Qt's QML formatter."""

import shutil
import subprocess
import sys


def main() -> int:
    formatter = shutil.which("qmllformat") or shutil.which("qmlformat")
    if formatter is None:
        raise RuntimeError("Qt QML formatter not found; install qmllformat or qmlformat")
    if len(sys.argv) < 2:
        return 0

    subprocess.run([formatter, "--inplace", *sys.argv[1:]], check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
