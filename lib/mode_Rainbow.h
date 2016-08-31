// Hue: ax + bt + c * sin8(dx + et)
u8 h_a, h_b, h_c, h_d, h_e;

// Value: sin8(ax + sin8(bx + ct) / d)
u8 v_a, v_b, v_c, v_d;

void mode_Rainbow_setup() {
	// nothing to do
}

// Separate RNG for rainbow patterns so that the different LED strips remain
// synchronized.  Lifted from FastLED lib8tion code.

uint16_t mode_Rainbow_seed = 33;

uint8_t mode_Rainbow_random8() {
    mode_Rainbow_seed = (mode_Rainbow_seed * (u16)2053) + (u16)13849;
    // return the sum of the high and low bytes, for better
    //  mixing and non-sequential correlation
    return (uint8_t)(((uint8_t)(mode_Rainbow_seed & 0xFF)) +
                     ((uint8_t)(mode_Rainbow_seed >> 8)));
}

void mode_Rainbow_activate() {
	// If several of the hue coefficients are large, the colors change too
	// quickly to look really nice
	do {
		h_a = mode_Rainbow_random8() % 3 + 1;
		h_b = mode_Rainbow_random8() % 2 + 1;
		h_c = mode_Rainbow_random8() % 2 + 1;
		h_d = mode_Rainbow_random8() % 4 + 1;
		h_e = mode_Rainbow_random8() % 2 + 1;
	} while (h_a + h_b + h_c + h_d + h_e > 10);

	v_a = mode_Rainbow_random8() % 3 + 1;
	v_b = mode_Rainbow_random8() % 5 + 1;
	v_c = mode_Rainbow_random8() % 4 + 1;
	v_d = mode_Rainbow_random8() % 3 + 1;
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

	return 15 * (h_b + h_c);
}
