#define INIT_FRAMES 5
#define INIT_MS 2500

void init_runSequence() {
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
