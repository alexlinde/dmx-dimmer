#include <Arduino.h>
#include <ArduinoRS485.h>
#include <ArduinoDMX.h>

// #define DEBUG

const uint16_t DEBOUNCE_MS = 20;
const uint16_t LONG_PRESS_MS = 500;
const uint16_t RETRANSMIT_MS = 2000;

const int universeSize = 16;
const uint16_t FIXTURES = 4;
const uint32_t buttonPins[FIXTURES] = {D1, D2, D3, D7};
const uint16_t fixtureAddr[FIXTURES] = {1, 5, 9, 13};

uint32_t buttonTime[FIXTURES];
bool lightState[FIXTURES];
uint8_t lightValue[FIXTURES];

void updateDMX() {
  #ifdef DEBUG
  // log the light state and value, '-' if the light is off, and value if on
  for (uint16_t i = 0; i < FIXTURES; i++) {
    if (lightState[i]) {
      Serial.print(lightValue[i], HEX);
    } else {
      Serial.print("--");
    }
    Serial.print(" ");
  }
  Serial.println();
  #else
  DMX.beginTransmission();
  for (uint16_t i = 0; i < FIXTURES; i++) {
    DMX.write(fixtureAddr[i], lightState[i] ? lightValue[i] : 0);
  }
  DMX.endTransmission();
  #endif
}

void setup() {
  // initialize button state
  for (uint16_t i = 0; i < FIXTURES; i++)
    pinMode(buttonPins[i], INPUT_PULLUP);
  memset(buttonTime, 0, sizeof(buttonTime));

  #ifdef DEBUG
  Serial.begin(9600);
  #else
  DMX.begin(universeSize);
  RS485.noReceive();
  #endif

  // initialize light state
  memset(lightState, 0, sizeof(lightState));
  memset(lightValue, 0, sizeof(lightValue));
  updateDMX();
}

void loop() {
  uint16_t retransmitTimer = 0;
  while (1) {
    bool changed = false;
    retransmitTimer++;

    // read buttons
    uint32_t lastReadTime = millis();
    for (uint16_t i = 0; i < FIXTURES; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        // keep track of how many debouce ticks it's pressed for 
        buttonTime[i]++;

        // Fade after long press
        if (buttonTime[i] > (LONG_PRESS_MS/DEBOUNCE_MS)) {
          if (lightValue[i] > 0) {
            lightState[i] = true;
            lightValue[i]--;
            changed = true;
          } else {
            // faded to off
            lightState[i] = false;
          }
        }
      }
      else {
        // Released before long press
        if (buttonTime[i] > 0 && buttonTime[i] <= (LONG_PRESS_MS/DEBOUNCE_MS)) {
          // toggle light state, and set to full brightness
          lightState[i] = !lightState[i];
          lightValue[i] = 255;
          changed = true;
        }
        buttonTime[i] = 0;
      }
    }

    // send if changed and retransmit periodically per DMX
    if (changed || retransmitTimer > (RETRANSMIT_MS / DEBOUNCE_MS)) {
      updateDMX();
      retransmitTimer = 0;
    }

    // debounce interval
    delay(DEBOUNCE_MS - (millis() - lastReadTime));
  }
}
