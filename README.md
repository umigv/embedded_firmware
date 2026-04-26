# ARV Embedded Firmware

## LED Status

Send a single ASCII character over serial (9600 baud) to set the mode:

| Char | Color/Mode |
|------|------------|
| `0`  | Off |
| `1`  | Solid blue (teleop) |
| `2`  | Flashing blue |
| `3`  | Flashing green |
| `4`  | Flashing yellow |
| `5`  | Flashing purple |
| `6`  | Flashing red |
| `9`  | Rainbow |

## Recovery Ultrasonic

Outputs one CSV line per cycle over serial (9600 baud):

```
s1,s2,s3
```

Each value is distance in cm. `999` means out of range / no echo.
