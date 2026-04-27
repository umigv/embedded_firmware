default:
    @just --list

# Install dependencies
setup:
    pip3 install platformio

# Build a project, or all projects if none specified: just build [project]
build project="":
    #!/usr/bin/env bash
    set -e
    if [ -z "{{project}}" ]; then
        for proj in $(find . -maxdepth 2 -name "platformio.ini" | xargs -I{} dirname {} | sort); do
            echo "=== Building $proj ==="
            (cd "$proj" && pio run)
        done
    else
        cd "{{project}}" && pio run
    fi

# Upload to board: just upload [project]
upload project:
    cd {{project}} && pio run --target upload

# Open serial monitor: just monitor [project]
monitor project:
    cd {{project}} && pio device monitor

# Clean a project, or all projects if none specified: just clean [project]
clean project="":
    #!/usr/bin/env bash
    set -e
    if [ -z "{{project}}" ]; then
        for proj in $(find . -maxdepth 2 -name "platformio.ini" | xargs -I{} dirname {} | sort); do
            echo ""
            echo "=== Cleaning $proj ==="
            (cd "$proj" && pio run --target clean)
        done
    else
        cd "{{project}}" && pio run --target clean
    fi

# Scaffold a new project: just new [project]
new project:
    #!/usr/bin/env bash
    set -e
    if [ -d "{{project}}" ]; then
        echo "Error: {{project}} already exists"
        exit 1
    fi
    mkdir -p "{{project}}/src"
    {
        echo '# Uncomment and fill in your board config:'
        echo '#'
        echo '# [env:esp32dev]'
        echo '# platform = espressif32'
        echo '# board = esp32dev'
        echo '# framework = arduino'
        echo '# monitor_speed = 9600'
        echo '#'
        echo '# [env:megaatmega2560]'
        echo '# platform = atmelavr'
        echo '# board = megaatmega2560'
        echo '# framework = arduino'
        echo '# monitor_speed = 9600'
        echo '#'
        echo '# For other boards: https://registry.platformio.org/search?t=platform'
    } > "{{project}}/platformio.ini"
    {
        echo '#include <Arduino.h>'
        echo ''
        echo 'void setup() {'
        echo ''
        echo '}'
        echo ''
        echo 'void loop() {'
        echo ''
        echo '}'
    } > "{{project}}/src/main.cpp"
    echo "Created {{project}} — fill in platformio.ini with your board config"

# List available serial ports
ports:
    pio device list

# Run clang-format on all source files (requires clang-format)
format:
    find . -path '*/src/*.cpp' | xargs clang-format -i

# Check formatting without modifying files
lint:
    find . -path '*/src/*.cpp' | xargs clang-format --dry-run --Werror
