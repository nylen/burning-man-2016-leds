#include <FastLED.h>

#define PIN_LEDS_0 2
#define PIN_LEDS_1 3

#define NUM_LEDS_PER_STRIP  150
#define NUM_STRIPS            2
//                         -----
#define NUM_LEDS            300

CRGB leds0[NUM_LEDS_PER_STRIP];
CRGB leds1[NUM_LEDS_PER_STRIP];

#define u8 uint8_t
#define u16 uint16_t

template<uint8_t DATA_PIN> class ALMOST_NEOPIXEL : public WS2812Controller800Khz<DATA_PIN, GRB> {};

void setAll(CRGB color) {
  for (u8 i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds0[i] = color;
    leds1[i] = color;
  }
}

void setLed(u16 i, CRGB color) {
  if (i < NUM_LEDS_PER_STRIP) {
    // Left side (reversed)
    leds0[NUM_LEDS_PER_STRIP - 1 - i] = color;
  } else if (i < 2 * NUM_LEDS_PER_STRIP) {
    // Right side
    leds1[i - NUM_LEDS_PER_STRIP] = color;
  }
}

void frame(int delayMs) {
static unsigned long lastFrame = 0;
  if (delayMs < 0) {
    return;
  }
  unsigned long now = millis();
  if (now - lastFrame < delayMs) {
    delay(delayMs - (now - lastFrame));
  }
  FastLED.show();
  lastFrame = millis();
}

#define INIT_FRAMES 5
#define INIT_MS 2500

#define NUM_MODES 1
#define MS_PER_MODE (10 * 60 * 1000)
u8 currentMode = 0;

#define MODE_SNAKES 0
#include "mode_Snakes.h"

void setup() {
  FastLED.addLeds<ALMOST_NEOPIXEL, PIN_LEDS_0>(leds0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<ALMOST_NEOPIXEL, PIN_LEDS_1>(leds1, NUM_LEDS_PER_STRIP);

  mode_Snakes_setup();

  FastLED.setBrightness(255);

  // Initialization sequence: show colors at edges,
  // white everywhere else, and blink a section of leds1
  CRGB white = CRGB(200, 150, 80);
  setAll(white);
  for (u8 i = 0; i < 6; i++) {
    CRGB color = CHSV(i * (255 / 7), 255, 255);
    setLed(i, color);
    setLed(NUM_LEDS_PER_STRIP - 1 - i, color);
    setLed(NUM_LEDS_PER_STRIP + i, color);
    setLed(NUM_LEDS - 1 - i, color);
  }
  for (u8 initFrame = 0; initFrame < INIT_FRAMES; initFrame++) {
    for (u8 i = 10; i < 50; i++) {
     if (initFrame % 2) {
      setLed(NUM_LEDS_PER_STRIP + i, CRGB::Black);
     } else {
      setLed(NUM_LEDS_PER_STRIP + i, white);
     }
   }
   frame(0);
   delay(INIT_MS / INIT_FRAMES);
  }
  setAll(CRGB::Black);
}

void loop() {
  currentMode = (millis() / MS_PER_MODE) % NUM_MODES;
  switch (currentMode) {
    case MODE_SNAKES:
      mode_Snakes_loop();
      break;
  }
}

