import argparse
import subprocess
import sys

from common import projects

EXTENSIONS = {".c", ".cpp", ".h", ".hpp"}


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Format all C/C++ sources with clang-format. With --check, report violations without modifying."
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Fail on formatting violations instead of fixing them",
    )
    args = parser.parse_args()

    sources = [
        str(path)
        for project in projects()
        for path in sorted((project / "src").rglob("*"))
        if path.suffix in EXTENSIONS
    ]

    flags = ["--dry-run", "--Werror"] if args.check else ["-i"]
    sys.exit(subprocess.run(["clang-format", *flags, *sources]).returncode)


if __name__ == "__main__":
    main()
