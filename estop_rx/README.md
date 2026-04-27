# estop_rx — LoRa E-Stop Receiver

ESP32-based receiver mounted on the robot. Controls a motor drive enable line and a warning LED based on packets from `estop_tx`.

## Hardware

| Pin | Connection |
|---|---|
| 5 | RFM95 CS |
| 26 | RFM95 RST |
| 22 | RFM95 INT |
| 4 | Warning LED |
| 16 | ODrive enable (HIGH = enabled) |

## Behavior

| Condition | ODRV pin | LED |
|---|---|---|
| SAFE packet received | HIGH | OFF |
| STOP packet received | LOW | ON |

Sends `"Ack"` back to TX after every received packet.

## Serial Output (9600 baud)

```
1   ← STOP received
0   ← SAFE received
```
