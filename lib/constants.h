////////////
// Colors //
////////////

// Colors for Sky background
CRGB green = CRGB(100, 255,  0);
CRGB blue  = CRGB(  0, 150, 80);
CRGB white = CRGB(200, 150, 50);

// Colors for hit animation
CRGB red    = CRGB(255,   0,   0);
CRGB yellow = CRGB(200, 150,   0);
CRGB orange = CRGB(225,  50,   0);

//////////////////////
// EEPROM addresses //
//////////////////////

// Drift for this chip, ms per hour
#define ADDRESS_DRIFT      0
// Current runtime
#define ADDRESS_MINUTES_0  2
// Last runtime
#define ADDRESS_MINUTES_1  4
// Runtime before last
#define ADDRESS_MINUTES_2  6
// ...
#define ADDRESS_MINUTES_3  8

// Sensor configuration (0 = none)
#define ADDRESS_SENSOR_CONFIG  10
