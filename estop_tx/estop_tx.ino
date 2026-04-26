// LoRa E-Stop TX (With Reply)
// -*- mode: C++ -*-
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
const uint8_t ESTOP_SAFE      = 0x00;

uint8_t state;

RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  pinMode(LED, OUTPUT);
  // Assuming switch connects Pin 16 to Ground when pressed
  pinMode(SW, INPUT_PULLUP); 

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
}

void loop() {
  uint8_t data_packet[1];
  
  // 1. READ SWITCH & SET LED
  if (digitalRead(SW) == LOW) {
    data_packet[0] = ESTOP_TRIGGERED;
    digitalWrite(LED, HIGH); // TX LED ON (Stop Pressed)
    Serial.println("Sending: STOP (0xFF)");
  } 
  else {
    data_packet[0] = ESTOP_SAFE;
    digitalWrite(LED, LOW);  // TX LED OFF
    Serial.println("Sending: SAFE (0x00)");
  }

  // 2. SEND PACKET
  rf95.send(data_packet, 1);
  rf95.waitPacketSent();
  
  // 3. WAIT FOR REPLY (1 Second Timeout)
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  if (rf95.waitAvailableTimeout(1000)) // 
  {
    if (rf95.recv(buf, &len)) {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    } else {
      Serial.println("Receive failed");
    }
  }
  else {
    Serial.println("No reply (Timeout)");
  }
  
  // Small delay before next loop
  delay(100); 
}