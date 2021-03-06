u16 framesThisMode = 0;

#include "mode_Snakes.h"
#define MODE_SNAKES 0
#include "mode_Sky.h"
#define MODE_SKY 1
#include "mode_Sparkles.h"
#define MODE_SPARKLES 2
#include "mode_Rainbow.h"
#define MODE_RAINBOW 3

#define NUM_MODES 4

u8 currentMode = NUM_MODES;
u8 lastMode = NUM_MODES;

u8 MODE_STEPS[] = {
	MODE_SKY,
	MODE_SPARKLES,
	MODE_SNAKES,
	MODE_RAINBOW,
	NUM_MODES // array terminator
};

#define MS_PER_MODE_STEP ((u32)2 * 60 * 1000)
u8 currentModeStep = 0;
u8 NUM_MODE_STEPS = 1; // calculated in setup()

void modes_setup() {
	while (MODE_STEPS[NUM_MODE_STEPS] < NUM_MODES) {
		NUM_MODE_STEPS++;
	}

	mode_Snakes_setup();
	mode_Sky_setup();
	mode_Sparkles_setup();
	mode_Rainbow_setup();
}

u16 modes_loop() {
	currentModeStep = (ms / MS_PER_MODE_STEP) % NUM_MODE_STEPS;

	currentMode = MODE_STEPS[currentModeStep];
	if (currentMode != lastMode) {
		switch (currentMode) {
			case MODE_SNAKES:
				mode_Snakes_activate();
				break;
			case MODE_SKY:
				mode_Sky_activate();
				break;
			case MODE_SPARKLES:
				mode_Sparkles_activate();
				break;
			case MODE_RAINBOW:
				mode_Rainbow_activate();
				break;
		}
		lastMode = currentMode;
		framesThisMode = 0;
	}

	u16 frameMs = 0;
	switch (currentMode) {
		case MODE_SNAKES:
			frameMs = mode_Snakes_loop();
			break;
		case MODE_SKY:
			frameMs = mode_Sky_loop();
			break;
		case MODE_SPARKLES:
			frameMs = mode_Sparkles_loop();
			break;
		case MODE_RAINBOW:
			frameMs = mode_Rainbow_loop();
			break;
	}

	if (framesThisMode < 65535) {
		framesThisMode++;
	}

	return frameMs;
}
