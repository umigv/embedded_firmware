// LoRa E-Stop RX (State Tracking - LED ON when Stopped)
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 5
#define RFM95_RST 26
#define RFM95_INT 22
#define LED 4
#define ODRV 16

#define RF95_FREQ 915.0

const uint8_t ESTOP_TRIGGERED = 0xFF;  // Sent when button is pressed
const uint8_t ESTOP_SAFE = 0x00;       // Sent when button is released (Adjust if your TX uses a different byte)

// Track the last known state of the transmitter. We start assuming it's safe.
uint8_t lastReceivedState = ESTOP_SAFE;

#define SERIAL_HEARTBEAT_MS 500
unsigned long lastHeartbeatTime = 0;

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(ODRV, OUTPUT);
    pinMode(RFM95_RST, OUTPUT);

    // Initialize to the SAFE state on boot
    digitalWrite(ODRV, HIGH);  // Motor drive enabled
    digitalWrite(LED, LOW);    // Warning LED OFF

    digitalWrite(RFM95_RST, HIGH);
    Serial.begin(9600);
    delay(100);

    // Manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!rf95.init())
        while (1);
    if (!rf95.setFrequency(RF95_FREQ))
        while (1);

    rf95.setTxPower(23, false);
}

void loop() {
    unsigned long now = millis();
    if (now - lastHeartbeatTime >= SERIAL_HEARTBEAT_MS) {
        Serial.println(lastReceivedState == ESTOP_TRIGGERED ? "1" : "0");
        lastHeartbeatTime = now;
    }

    if (rf95.available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf95.recv(buf, &len)) {
            if (len > 0) {
                uint8_t incomingState = buf[0];

                // ONLY execute if the transmitter's state has CHANGED
                if (incomingState != lastReceivedState) {
                    if (incomingState == ESTOP_TRIGGERED) {
                        // Button was just PRESSED (System is STOPPED)
                        digitalWrite(ODRV, LOW);  // Disable the motor/drive
                        digitalWrite(LED, HIGH);  // Turn ON the warning LED
                        Serial.println("1");      // Outputs '1' ONCE until released
                    } else if (incomingState == ESTOP_SAFE) {
                        // Button was just RELEASED (System is SAFE)
                        digitalWrite(ODRV, HIGH);  // Enable the motor/drive
                        digitalWrite(LED, LOW);    // Turn OFF the warning LED
                        Serial.println("0");       // Outputs '0' ONCE until pressed
                    }

                    // Update our tracker to the new state
                    lastReceivedState = incomingState;

                    // Brief lockout to prevent double-triggers from signal bounce
                    delay(50);
                }
            }

            // Always send the Ack so the TX knows we are alive
            uint8_t data[] = "Ack";
            rf95.send(data, sizeof(data));
            rf95.waitPacketSent();
        }
    }
}
