import argparse
import json
import subprocess
import sys
from pathlib import Path

from common import ROOT, projects

# GCC-only flags from the ESP32/AVR cross-toolchains; clangd's clang parser rejects them
GCC_ONLY_FLAGS = {"-mlongcalls", "-fstrict-volatile-bitfields", "-fno-tree-switch-conversion"}


def sanitize_compiledb(project: Path) -> None:
    db = project / "compile_commands.json"
    entries = json.loads(db.read_text(encoding="utf-8"))
    for entry in entries:
        entry["command"] = " ".join(arg for arg in entry["command"].split() if arg not in GCC_ONLY_FLAGS)
    db.write_text(json.dumps(entries, indent=2), encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Run a PlatformIO target for one project, or for every project if none is given."
    )
    parser.add_argument("project", nargs="?", help="Project directory (default: all projects)")
    parser.add_argument("--target", help="PlatformIO target, e.g. clean or compiledb (default: build)")
    args = parser.parse_args()

    if args.project:
        dirs = [ROOT / args.project]
        if not (dirs[0] / "platformio.ini").exists():
            sys.exit(f"Error: no platformio.ini in {args.project}")
    else:
        dirs = projects()

    for project in dirs:
        # We flush here so this shows up together with the subprocess output
        print(f"=== {project.name} ===", flush=True)
        cmd = ["pio", "run", "-d", str(project)]
        if args.target:
            cmd += ["--target", args.target]
        result = subprocess.run(cmd)
        if result.returncode != 0:
            sys.exit(result.returncode)
        if args.target == "compiledb":
            sanitize_compiledb(project)


if __name__ == "__main__":
    main()
