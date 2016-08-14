u16 sparkleCounter = 0;
u16 sparkleIndex = 1;

// This is a complicated animation; we can save memory by using a pseudorandom
// sequence and "replaying" the necessary part of it during each loop

// https://oeis.org/A246866
#define XOR_MASK_512 0x0240

#define SPARKLE_FRAMES 30 // also the number of sparkles

// Animation transition points by frame index
#define SPARKLE_END_FADE        8
#define SPARKLE_END_RAINBOW     22
#define SPARKLE_END_DESATURATE  26
#define SPARKLE_END_EXPLODE     SPARKLE_FRAMES

#define SPARKLE_FRAMES_DESATURATE (SPARKLE_END_DESATURATE - SPARKLE_END_RAINBOW)
#define SPARKLE_FRAMES_EXPLODE    (SPARKLE_END_EXPLODE    - SPARKLE_END_DESATURATE)

void mode_Sparkles_setup() {
	// nothing to do
}

void mode_Sparkles_activate() {
	// Restart the sequence with black each time
	sparkleCounter = 0;
	// Set a random starting point (XOR sequence doesn't work with 0)
	do {
		sparkleIndex = random16() % NUM_LEDS;
	} while (sparkleIndex == 0);
}

void mode_Sparkles_loop() {
	setAll(CRGB::Black);

	u16 sparkleIndexTemp = sparkleIndex;

	u16 start = max(sparkleCounter, SPARKLE_FRAMES) - SPARKLE_FRAMES;

	for (u16 i = start; i <= sparkleCounter; i++) {
		u16 ledIndex = (sparkleIndexTemp + randOffset) % NUM_LEDS;

		// Frame index of this sparkle
		u8 frame = sparkleCounter - i;

		CRGB color = CHSV((ledIndex + frame) * 5, 255, 255);
		if (frame < SPARKLE_END_FADE) {
			// beginning; fade in
			color = blend(
				CRGB::Black,
				color,
				frame * (255 / SPARKLE_END_FADE)
			);
			setLed(ledIndex, color);
		} else if (frame < SPARKLE_END_RAINBOW) {
			// middle; rainbow shifting
			setLed(ledIndex, color);
		} else if (frame < SPARKLE_END_DESATURATE) {
			// before explosion effect; fade to white
			color = blend(
				white,
				color,
				(
					(SPARKLE_END_DESATURATE - frame) *
					(255 / SPARKLE_FRAMES_DESATURATE)
				)
			);
			setLed(ledIndex, color);
		} else {
			// end; explosion effect
			u8 distance =
				SPARKLE_FRAMES_EXPLODE -
				(SPARKLE_END_EXPLODE - frame);
			color = blend(
				white,
				CRGB::Black,
				distance * (255 / SPARKLE_FRAMES_EXPLODE)
			);
			u16 edge1 = max(ledIndex, distance) - distance;
			u16 edge2 = min(ledIndex, NUM_LEDS - distance) + distance;
			setLed(edge1, color);
			for (u16 j = edge1 + 1; j < edge2; j++) {
				setLed(j, CRGB::Black);
			}
			setLed(edge2, color);
		}

		sparkleIndexTemp =
			(sparkleIndexTemp >> 1) ^
			(sparkleIndexTemp & 1 ? XOR_MASK_512 : 0);
	}

	sparkleCounter++;
	if (sparkleCounter >= SPARKLE_FRAMES) {
		sparkleIndex =
			(sparkleIndex >> 1) ^
			(sparkleIndex & 1 ? XOR_MASK_512 : 0);
	}

	frame(20);
}
