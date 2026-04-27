# ARV Embedded Firmware

| Project | Board | Description |
|---|---|---|
| [`estop_tx`](estop_tx/README.md) | ESP32 | LoRa e-stop transmitter (handheld button) |
| [`estop_rx`](estop_rx/README.md) | ESP32 | LoRa e-stop receiver (on robot) |
| [`led_status`](led_status/README.md) | Arduino Mega | RGB LED strip status controller |
| [`recovery_ultrasonic`](recovery_ultrasonic/README.md) | Arduino Mega | Three HC-SR04 sensors, CSV serial output |

## Setup

```bash
just setup
```

**VS Code:** Install [clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) extensions (VS Code will prompt you automatically). Then run:

```bash
just intellisense
```

This generates `compile_commands.json` in each project for clangd IntelliSense. Re-run after modifying `platformio.ini` (e.g. adding a library or changing board).

## Build & Upload

```bash
# Build one project
just build estop_tx

# Build all projects
just build

# Upload to connected board
just upload estop_tx

# Open serial monitor
just monitor estop_rx

# List available serial ports
just ports
```

```bash
# Check formatting
just lint

# Auto-fix formatting
just format
```

Run `just` to list all available commands.

## Adding a New Project

```bash
just new my_project
```

Scaffolds `my_project/platformio.ini` and `my_project/src/main.cpp`. Fill in `platformio.ini` with your board:

```ini
; ESP32
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 9600

; Arduino Mega
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
monitor_speed = 9600
```

For other boards see the [PlatformIO board explorer](https://registry.platformio.org/search?t=platform).
