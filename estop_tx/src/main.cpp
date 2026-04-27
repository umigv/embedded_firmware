// LoRa E-Stop TX (Toggle Based with Debounce)
// -*- mode: C++ -*-
#include <Arduino.h>
#include <SPI.h>
#include <RH_RF95.h>

// --- PIN DEFINITIONS ---
#define RFM95_CS 5
#define RFM95_RST 26
#define RFM95_INT 22
#define LED 4
#define SW 16

#define RF95_FREQ 915.0

// Protocol: 0xFF = STOP, 0x00 = SAFE
const uint8_t ESTOP_TRIGGERED = 0xFF;
const uint8_t ESTOP_SAFE = 0x00;

// --- TOGGLE TRACKING VARIABLES ---
bool estopActive = false;     // Tracks system state: false = SAFE, true = STOP
bool lastButtonState = HIGH;  // Tracks the physical pin state from the last loop

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(SW, INPUT_PULLUP);  // Pin 16 pulled HIGH, drops to LOW when pressed

    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    Serial.begin(9600);
    delay(100);

    // Manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);

    if (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }

    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }

    rf95.setTxPower(23, false);

    // Sync LED with the initial safe state
    digitalWrite(LED, LOW);
}

void loop() {
    // 1. READ SWITCH & DETECT NEW PRESS (FALLING EDGE)
    bool currentButtonState = digitalRead(SW);

    // If button is currently LOW (pressed) but was HIGH (released) last time:
    if (currentButtonState == LOW && lastButtonState == HIGH) {
        // Toggle our state variable
        estopActive = !estopActive;

        // Update local LED immediately for instant visual feedback
        digitalWrite(LED, estopActive ? HIGH : LOW);

        Serial.print("Button Clicked! Toggled state to: ");
        Serial.println(estopActive ? "STOP (0xFF)" : "SAFE (0x00)");

        // Hardware Debounce: Wait out the physical contact switch bounce
        delay(50);
    }

    // Save the current state for comparison on the next loop execution
    lastButtonState = currentButtonState;

    // 2. PREPARE PACKET BASED ON TOGGLED STATE
    uint8_t data_packet[1];
    data_packet[0] = estopActive ? ESTOP_TRIGGERED : ESTOP_SAFE;

    // 3. SEND PACKET
    rf95.send(data_packet, 1);
    rf95.waitPacketSent();

    // 4. WAIT FOR REPLY (Reduced to 150ms for button responsiveness)
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.waitAvailableTimeout(150)) {
        if (rf95.recv(buf, &len)) {
            Serial.print("Got reply: ");
            Serial.println((char*)buf);
            Serial.print("RSSI: ");
            Serial.println(rf95.lastRssi(), DEC);
        } else {
            data_packet[0] = ESTOP_SAFE;
            digitalWrite(LED, LOW);  // TX LED OFF
            Serial.println("Sending: SAFE (0x00)");
        }
    } else {
        Serial.println("No reply (Timeout)");
    }

    // Small baseline delay to prevent resource flooding
    delay(10);
}
