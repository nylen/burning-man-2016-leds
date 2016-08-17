// Hue: ax + bt + c * sin8(dx + et)
u8 h_a, h_b, h_c, h_d, h_e;

// Value: sin8(ax + sin8(bx + ct) / d)
u8 v_a, v_b, v_c, v_d;

void mode_Rainbow_setup() {
	// nothing to do
}

void mode_Rainbow_activate() {
	h_a = random8() % 5 + 1;
	h_b = random8() % 4 + 1;
	h_c = random8() % 4 + 1;
	h_d = random8() % 8 + 1;
	h_e = random8() % 6 + 1;

	v_a = random8() % 3 + 1;
	v_b = random8() % 5 + 1;
	v_c = random8() % 4 + 1;
	v_d = random8() % 5 + 1;
}

u16 mode_Rainbow_loop() {
	if (framesThisMode < 64) {
		FastLED.setBrightness(framesThisMode * 4);
	} else if (framesThisMode == 64) {
		FastLED.setBrightness(255);
	}

	for (u16 i = 0; i < NUM_LEDS; i++) {
		u8 h = (
			(u16)h_a * i +
			(u16)h_b * t +
			(u16)h_c * sin8(
				(u16)h_d * i +
				(u16)h_e * t
			)
		);
		u8 s = 255;
		u8 v = sin8(
			(u16)v_a * i +
			sin8(
				(u16)v_b * i +
				(u16)v_c * t
			) / v_d
		);
		setLed(i, CHSV(h, s, v));
	}

	return 20;
}
