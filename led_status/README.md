# led_status
Receives a single ASCII character over serial to switch LED strip modes. All modes are non-blocking.

## Hardware
| Pin | Connection |
|---|---|
| 6 | WS2811 data line |

Strip: 30 LEDs, WS2811, GRB order.

## Serial input
Serial runs at 9600 baud. One ASCII character per command:
| Char | Mode |
|---|---|
| `0` | Off |
| `1` | Solid blue |
| `2` | Flashing blue |
| `3` | Flashing green |
| `4` | Flashing yellow |
| `5` | Flashing purple |
| `6` | Flashing red |
| `7` | Solid green |
| `9` | Rainbow |

Commands double as a heartbeat: if none arrives for 3 s (or none has arrived yet), the strip shows rainbow to signal a disconnected driver.

## Serial output
Prints `READY` once on boot. The driver should wait for this before sending commands because if a command is sent while the arduino is in the bootloader, the Arduino will be reset. If the driver sends this periodically it is possible that the firmware gets stuck in the bootloader forever. 
