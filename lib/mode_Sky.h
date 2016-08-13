// Colors for normal background
CRGB green = CRGB(100, 255,  0);
CRGB blue  = CRGB(  0, 150, 80);
CRGB white = CRGB(200, 150, 50);

// Colors for hit animation
CRGB red    = CRGB(255,   0,   0);
CRGB yellow = CRGB(200, 150,   0);
CRGB orange = CRGB(225,  50,   0);

void mode_Sky_setup() {
	// nothing to do
}

void mode_Sky_loop() {
	// Shifting between green/blue/white
	for (u16 i = 0; i < NUM_LEDS; i++) {
		u8 blueWeight = sin8(
			1 * randOffset +
			t / 2 +
			3 * i
		);
		u8 whiteWeight = sin8(
			2 * randOffset +
			t / 1 +
			2 * i
		) / 3;
		CRGB c = green;
		c = blend(c,  blue,  blueWeight);
		c = blend(c, white, whiteWeight);
		setLed(i, c);
	}
	// and occasionally some orange
	if ((t % 2048) < 512) {
		u8 minWeight = cubicwave8((t % 2048) / 2);
		for (u16 i = 0; i < NUM_LEDS; i++) {
			u8 orangeWeight = min(
				minWeight,
				cubicwave8(
					1 * randOffset +
					2 * (NUM_LEDS - i) +
					1 * (t % 2048)
				)
			);
			CRGB c = getLed(i);
			setLed(i, blend(c, orange, orangeWeight));
		}
	}

	frame(15);
}
