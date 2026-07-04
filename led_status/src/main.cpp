// led_status.ino
// Robot status LED strip controller.
// Receives a single ASCII character over serial to switch modes:
//   '0' -> Off
//   '1' -> Teleop: SOLID blue
//   '2' -> Flashing blue
//   '3' -> Flashing green
//   '4' -> Flashing yellow
//   '5' -> Flashing purple
//   '6' -> Flashing red
//   '9' -> Rainbow (animated)
//
// All modes are non-blocking — serial commands are responsive at all times,
// including during animations.

#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN 6
#define NUM_LEDS 30
#define BRIGHTNESS 100

// Animation timing (milliseconds).
const unsigned long BLINK_INTERVAL_MS = 500;
const unsigned long RAINBOW_INTERVAL_MS = 20;     // lower = faster scroll
const unsigned long HEARTBEAT_TIMEOUT_MS = 3000;  // rainbow if no command within this window

CRGB leds[NUM_LEDS];

// Mode state.
enum Mode { MODE_OFF, MODE_TELEOP_BLUE, MODE_BLUE, MODE_GREEN, MODE_YELLOW, MODE_PURPLE, MODE_RED, MODE_RAINBOW };
Mode currentMode = MODE_OFF;

// Animation state.
unsigned long lastBlinkToggleMs = 0;
unsigned long lastRainbowStepMs = 0;
unsigned long lastHeartbeatMs = 0;
bool blinkOn = false;
uint8_t rainbowHueOffset = 0;

void handleSerial();
void updateLeds(Mode mode);

void setup() {
    // Use GRB so named FastLED colors come out correct on WS2811 strips.
    FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
    Serial.begin(9600);
    // Boot handshake: led_subscriber.py blocks until it sees this line.
    Serial.println("READY");
}

void loop() {
    handleSerial();
    bool driverConnected = lastHeartbeatMs > 0 && (millis() - lastHeartbeatMs <= HEARTBEAT_TIMEOUT_MS);
    updateLeds(driverConnected ? currentMode : MODE_RAINBOW);
}

void handleSerial() {
    if (Serial.available() <= 0) return;

    char c = (char)Serial.read();
    Mode previousMode = currentMode;

    switch (c) {
        case '0':
            currentMode = MODE_OFF;
            break;
        case '1':
            currentMode = MODE_TELEOP_BLUE;
            break;
        case '2':
            currentMode = MODE_BLUE;
            break;
        case '3':
            currentMode = MODE_GREEN;
            break;
        case '4':
            currentMode = MODE_YELLOW;
            break;
        case '5':
            currentMode = MODE_PURPLE;
            break;
        case '6':
            currentMode = MODE_RED;
            break;
        case '9':
            currentMode = MODE_RAINBOW;
            break;
        default: /* ignore unknown chars (e.g. \r, \n) */
            return;
    }

    lastHeartbeatMs = millis();

    // On any mode change, reset blink so the new color starts ON immediately
    // instead of waiting up to 500 ms for the next toggle.
    if (currentMode != previousMode) {
        blinkOn = true;
        lastBlinkToggleMs = millis();
    }
}

// Returns the color associated with each blinking mode.
CRGB blinkColorFor(Mode m) {
    switch (m) {
        case MODE_BLUE:
            return CRGB::Blue;
        case MODE_GREEN:
            return CRGB::Green;
        case MODE_YELLOW:
            return CRGB::Yellow;
        case MODE_PURPLE:
            return CRGB::Purple;
        case MODE_RED:
            return CRGB::Red;
        default:
            return CRGB::Black;
    }
}

bool isBlinkingMode(Mode m) {
    return m == MODE_BLUE || m == MODE_GREEN || m == MODE_YELLOW || m == MODE_PURPLE || m == MODE_RED;
}

void updateLeds(Mode mode) {
    unsigned long now = millis();

    if (mode == MODE_OFF) {
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        FastLED.show();
        return;
    }

    if (mode == MODE_TELEOP_BLUE) {
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        return;
    }

    if (mode == MODE_RAINBOW) {
        if (now - lastRainbowStepMs >= RAINBOW_INTERVAL_MS) {
            lastRainbowStepMs = now;
            rainbowHueOffset++;
            for (int i = 0; i < NUM_LEDS; i++) {
                leds[i] = CHSV(rainbowHueOffset + (i * 256 / NUM_LEDS), 255, 255);
            }
            FastLED.show();
        }
        return;
    }

    if (isBlinkingMode(mode)) {
        if (now - lastBlinkToggleMs >= BLINK_INTERVAL_MS) {
            lastBlinkToggleMs = now;
            blinkOn = !blinkOn;
            fill_solid(leds, NUM_LEDS, blinkOn ? blinkColorFor(mode) : CRGB::Black);
            FastLED.show();
        }
    }
}
