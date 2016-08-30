#include <EEPROM.h>
#include <FastLED.h>

#define PIN_LEDS_0 2
#define PIN_LEDS_1 3

#define NUM_LEDS_PER_STRIP  150
#define NUM_STRIPS            2
//                         -----
#define NUM_LEDS            300

CRGB leds0[NUM_LEDS_PER_STRIP];
CRGB leds1[NUM_LEDS_PER_STRIP];

#define u8  uint8_t
#define s8  int8_t
#define u16 uint16_t
#define s16 int16_t
#define u32 uint32_t
#define s32 int32_t

u16 MIN_LED_INDEX = 0;
u16 MAX_LED_INDEX = NUM_LEDS - 1;

template<uint8_t DATA_PIN> class WS2812_STRIP : public WS2812Controller800Khz<DATA_PIN, GRB> {};

// For differentiating effects between boards
u8 randOffset = 0;

#include "lib/constants.h"
#include "lib/graphics.h"
#include "lib/init.h"
#include "lib/sensors.h"
#include "lib/timing.h"

#include "lib/modes.h"

#include "lib/commands.h"

void setup() {
  FastLED.addLeds<WS2812_STRIP, PIN_LEDS_0>(leds0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812_STRIP, PIN_LEDS_1>(leds1, NUM_LEDS_PER_STRIP);

  FastLED.setBrightness(255);
  Serial.begin(115200);

  // More randomness
  random16_add_entropy(analogRead(A0));
  random16_add_entropy(analogRead(A5));
  randOffset = random8();

  sensors_setup();
  timing_setup();

  modes_setup();

  // Short graphics sequence for testing
  init_runSequence();
}

void loop() {
  commands_loop();
  timing_loop();
  u16 frameMs = modes_loop();

  sensors_loop(frameMs);
  frame(frameMs);
}
