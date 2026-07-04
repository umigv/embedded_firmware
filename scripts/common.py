from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def projects() -> list[Path]:
    """Return every PlatformIO project directory, sorted by name.

    Includes template_project, so the template is built and linted like any other project.
    """
    return sorted(ini.parent for ini in ROOT.glob("*/platformio.ini"))
