import argparse
import re
import shutil
import sys

from common import ROOT

TEMPLATE = ROOT / "template_project"


def main() -> None:
    parser = argparse.ArgumentParser(description="Scaffold a new PlatformIO project by copying template_project.")
    parser.add_argument("project", help="Name of the project directory to create")
    args = parser.parse_args()

    if not re.match(r"^[a-z][a-z0-9_]*$", args.project):
        sys.exit(
            f"Error: Invalid project name '{args.project}'. "
            "Must start with a lowercase letter and contain only lowercase letters, numbers, and underscores."
        )

    project = ROOT / args.project
    if project.exists():
        sys.exit(f"error: {args.project} already exists")

    # the template may have local build/index output (.pio, .cache, compile_commands.json); don't copy it
    shutil.copytree(TEMPLATE, project, ignore=shutil.ignore_patterns(".*", "compile_commands.json"))
    for path in project.rglob("*"):
        if path.is_file():
            text = path.read_text(encoding="utf-8")
            path.write_text(text.replace(TEMPLATE.name, args.project), encoding="utf-8")

    print(f"Created {args.project}. Check that platformio.ini matches your board")


if __name__ == "__main__":
    main()
