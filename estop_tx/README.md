# estop_tx — LoRa E-Stop Transmitter

ESP32-based handheld transmitter. Sends a STOP or SAFE packet over LoRa every loop and waits for an ack from `estop_rx`.

## Hardware

| Pin | Connection |
|---|---|
| 5 | RFM95 CS |
| 26 | RFM95 RST |
| 22 | RFM95 INT |
| 4 | Status LED |
| 16 | E-stop switch (to GND when pressed) |

## Protocol

| Byte | Meaning |
|---|---|
| `0xFF` | STOP — button pressed |
| `0x00` | SAFE — button released |

## Serial Output (9600 baud)

```
Sending: STOP (0xFF)
Sending: SAFE (0x00)
Got reply: Ack
RSSI: -65
No reply (Timeout)
```
