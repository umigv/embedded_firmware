# recovery_ultrasonic
Reads three HC-SR04 ultrasonic sensors and streams the minimum of the three distances over serial. Compatible with Arduino Serial Plotter.

## Hardware
| Pin | Connection |
|---|---|
| 9 | Sensor 1 TRIG |
| 10 | Sensor 1 ECHO |
| 11 | Sensor 2 TRIG |
| 12 | Sensor 2 ECHO |
| 2 | Sensor 3 TRIG |
| 3 | Sensor 3 ECHO |

## Serial output
Serial runs at 9600 baud. One line per cycle: the minimum of the three distances, in cm. `0` means no echo (`pulseIn` timeout).
