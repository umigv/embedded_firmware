# just's default recipe shell on Windows is sh, which isn't on PATH on stock machines
set windows-shell := ["powershell.exe", "-NoLogo", "-Command"]

default:
    @just --list

# Install dependencies and generate clangd compile databases
setup:
    pixi install
    just intellisense

# Build a project, or all projects if none specified
build project="":
    pixi run python scripts/pio_run.py {{project}}

# Upload to board (extra flags passed through, e.g. --upload-port COM5)
upload project *args:
    pixi run pio run -d {{project}} --target upload {{args}}

# Open serial monitor (extra flags passed through, e.g. --port COM5)
monitor project *args:
    pixi run pio device monitor -d {{project}} {{args}}

# Clean a project, or all projects if none specified
clean project="":
    pixi run python scripts/pio_run.py --target clean {{project}}

# Create a new project from template_project
create-project project:
    pixi run python scripts/create_project.py {{project}}

# Regenerate compile_commands.json for clangd (run after a platformio.ini changes or on stale errors)
intellisense:
    pixi run python scripts/pio_run.py --target compiledb

# List available serial ports
ports:
    pixi run pio device list

# Run clang-format on all source files
format:
    pixi run python scripts/format.py

# Check formatting without modifying files
lint:
    pixi run python scripts/format.py --check
