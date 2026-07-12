# estop_rx
E-stop receiver mounted on the robot. Controls a motor drive enable line and a warning LED based on packets from `estop_tx`.

## Hardware
| Pin | Connection |
|---|---|
| 5 | RFM95 CS |
| 26 | RFM95 RST |
| 22 | RFM95 INT |
| 4 | Warning LED |
| 16 | Motor drive enable (HIGH = enabled) |

## LoRa protocol
| Packet received | Motor drive pin | LED |
|---|---|---|
| SAFE (`0x00`) | HIGH | OFF |
| STOP (`0xFF`) | LOW | ON |

Sends `"Ack"` back to TX after every received packet.

## Serial output
Serial runs at 9600 baud. Prints the current state as a heartbeat every 500 ms, plus once immediately on every state change:
```
1   ← STOP active
0   ← SAFE
```
