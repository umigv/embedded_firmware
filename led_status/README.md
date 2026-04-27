# led_status — RGB LED Strip Controller

Arduino Mega. Receives a single ASCII character over serial to switch LED modes. All modes are non-blocking.

## Hardware

| Pin | Connection |
|---|---|
| 6 | WS2811 data line |

Strip: 30 LEDs, WS2811, GRB order.

## Serial Commands (9600 baud)

| Char | Mode |
|---|---|
| `0` | Off |
| `1` | Solid blue (teleop) |
| `2` | Flashing blue |
| `3` | Flashing green |
| `4` | Flashing yellow |
| `5` | Flashing purple |
| `6` | Flashing red |
| `9` | Rainbow |
