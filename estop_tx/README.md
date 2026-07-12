# estop_tx
Handheld e-stop transmitter. Each button press toggles between STOP and SAFE. The current state is sent over LoRa every loop, waiting for an ack from `estop_rx`.

## Hardware
| Pin | Connection |
|---|---|
| 5 | RFM95 CS |
| 26 | RFM95 RST |
| 22 | RFM95 INT |
| 4 | Status LED (ON while STOP active) |
| 16 | E-stop button (to GND when pressed) |

## LoRa protocol
| Byte | Meaning |
|---|---|
| `0xFF` | STOP (e-stop active) |
| `0x00` | SAFE |

## Serial output
Serial runs at 9600 baud.
```
Button Clicked! Toggled state to: STOP (0xFF)
Got reply: Ack
RSSI: -65
No reply (Timeout)
```
