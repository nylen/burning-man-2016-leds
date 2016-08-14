#define NUM_SPARKLES 30
u16 sparklePositions[NUM_SPARKLES];
u8 sparkleFrames[NUM_SPARKLES];

#define SPARKLE_FRAMES 50

// Animation transition points by frame index
#define SPARKLE_END_FADE        8
#define SPARKLE_END_RAINBOW     42
#define SPARKLE_END_DESATURATE  46
#define SPARKLE_END_EXPLODE     SPARKLE_FRAMES

#define SPARKLE_FRAMES_DESATURATE (SPARKLE_END_DESATURATE - SPARKLE_END_RAINBOW)
#define SPARKLE_FRAMES_EXPLODE    (SPARKLE_END_EXPLODE    - SPARKLE_END_DESATURATE)

void mode_Sparkles_setup() {
	// nothing to do
}

void mode_Sparkles_activate() {
	// Restart the sequence with black each time
	for (u8 i = 0; i < NUM_SPARKLES; i++) {
		sparklePositions[i] = NUM_LEDS;
	}
}

void mode_Sparkles_loop() {
	setAll(CRGB::Black);

	for (u8 i = 0; i < NUM_SPARKLES; i++) {
		u16 pos = sparklePositions[i];
		if (pos < NUM_LEDS) {
			u8 frame = sparkleFrames[i];

			CRGB color = CHSV((pos + frame) * 5, 255, 255);
			if (frame < SPARKLE_END_FADE) {
				// beginning; fade in
				color = blend(
					CRGB::Black,
					color,
					frame * (255 / SPARKLE_END_FADE)
				);
				setLed(pos, color);
			} else if (frame < SPARKLE_END_RAINBOW) {
				// middle; rainbow shifting
				setLed(pos, color);
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
				setLed(pos, color);
			} else {
				// end; explosion effect
				if (frame == SPARKLE_END_EXPLODE - SPARKLE_FRAMES_EXPLODE) {
					// Explosions kill all nearby sparkles
					for (u8 j = 0; j < NUM_SPARKLES; j++) {
						u16 pos2 = sparklePositions[j];
						if (
							j != i &&
							pos2 + SPARKLE_FRAMES_EXPLODE >= pos &&
							pos2 <= pos + SPARKLE_FRAMES_EXPLODE
						) {
							sparklePositions[j] = NUM_LEDS;
						}
					}
				}
				u8 distance =
					SPARKLE_FRAMES_EXPLODE -
					(SPARKLE_END_EXPLODE - frame);
				color = blend(
					white,
					CRGB::Black,
					distance * (255 / SPARKLE_FRAMES_EXPLODE)
				);
				u16 edge1 = max(pos, distance) - distance;
				u16 edge2 = min(pos, NUM_LEDS - distance) + distance;
				setLed(edge1, color);
				for (u16 j = edge1 + 1; j < edge2; j++) {
					setLed(j, CRGB::Black);
				}
				setLed(edge2, color);
			}

			if (++sparkleFrames[i] == SPARKLE_FRAMES) {
				sparklePositions[i] = NUM_LEDS;
			}
		}
	}

	if (random8() < 32) {
		for (u8 i = 0; i < NUM_SPARKLES; i++) {
			if (sparklePositions[i] == NUM_LEDS) {
				sparklePositions[i] = random16() % NUM_LEDS;
				sparkleFrames[i] = 0;
				break;
			}
		}
	}

	frame(20);
}
