void setAll(CRGB color) {
	for (u8 i = 0; i < NUM_LEDS_PER_STRIP; i++) {
		leds0[i] = color;
		leds1[i] = color;
	}
}

CRGB getLed(u16 i) {
	if (i < NUM_LEDS_PER_STRIP) { // Left side (reversed)
		return leds0[NUM_LEDS_PER_STRIP - 1 - i];
	} else if (i < 2 * NUM_LEDS_PER_STRIP) { // Right side
		return leds1[i - NUM_LEDS_PER_STRIP];
	}
}

void setLed(u16 i, CRGB color) {
	if (i < NUM_LEDS_PER_STRIP) { // Left side (reversed)
		leds0[NUM_LEDS_PER_STRIP - 1 - i] = color;
	} else if (i < 2 * NUM_LEDS_PER_STRIP) { // Right side
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
