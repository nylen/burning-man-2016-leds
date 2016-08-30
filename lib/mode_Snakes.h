#define NUM_SNAKES 5
#define SNAKE_LENGTH 15

u8 snakeHues[NUM_SNAKES];
u16 snakePositions[NUM_SNAKES];

void mode_Snakes_setup() {
	// nothing to do
}

void mode_Snakes_activate() {
	// Restart the sequence with black each time
	for (u8 i = 0; i < NUM_SNAKES; i++) {
		snakePositions[i] = NUM_LEDS;
	}
}

u16 mode_Snakes_loop() {
	setAll(CRGB::Black);

	u16 minSnakePosition = NUM_LEDS;

	for (u8 i = 0; i < NUM_SNAKES; i++) {
		if (snakePositions[i] < NUM_LEDS) {
			CRGB color = CHSV(snakeHues[i], 255, 255);
			for (u8 j = 0; j < SNAKE_LENGTH; j++) {
				setLed(snakePositions[i] + j, color);
				if (minSnakePosition > snakePositions[i]) {
					minSnakePosition = snakePositions[i];
				}
			}
			snakePositions[i]++;
		}
	}

	u8 threshold = min(framesThisMode, 160) / 16;
	if (random8() < threshold) {
		for (u8 i = 0; i < NUM_SNAKES; i++) {
			if (snakePositions[i] == NUM_LEDS) {
				if (minSnakePosition > SNAKE_LENGTH + 5) {
					snakePositions[i] = MIN_LED_INDEX;
					minSnakePosition = MIN_LED_INDEX;
					snakeHues[i] = random8();
					break;
				}
			}
		}
	}

	return 8;
}
