# recovery_ultrasonic — Ultrasonic Sensor Array

Arduino Mega. Reads three HC-SR04 sensors and streams CSV over serial. Compatible with Arduino Serial Plotter.

## Hardware

| Pin | Connection |
|---|---|
| 9 | Sensor 1 TRIG |
| 10 | Sensor 1 ECHO |
| 11 | Sensor 2 TRIG |
| 12 | Sensor 2 ECHO |
| 2 | Sensor 3 TRIG |
| 3 | Sensor 3 ECHO |

## Serial Output (9600 baud)

One line per cycle (~260ms):
```
s1,s2,s3
```

Values are distance in cm. `999` = no echo / out of range (>5m).
