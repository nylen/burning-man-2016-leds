#define MS_PER_MODE_STEP (10 /* * 60 */ * 1000)
u8 currentModeStep = 0;
u8 NUM_MODE_STEPS = 1; // calculated in setup()

#include "mode_Snakes.h"
#define MODE_SNAKES 0
#include "mode_Sky.h"
#define MODE_SKY 1
#include "mode_Sparkles.h"
#define MODE_SPARKLES 2

#define NUM_MODES 3

u8 currentMode = NUM_MODES;
u8 lastMode = NUM_MODES;

u8 MODE_STEPS[] = {
	MODE_SKY, MODE_SKY,
	MODE_SPARKLES,
	MODE_SKY, MODE_SKY,
	MODE_SNAKES,
	NUM_MODES // array terminator
};

void modes_setup() {
	while (MODE_STEPS[NUM_MODE_STEPS] < NUM_MODES) {
		NUM_MODE_STEPS++;
	}
}

void modes_loop() {
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
		}
		lastMode = currentMode;
	}

	switch (currentMode) {
		case MODE_SNAKES:
			mode_Snakes_loop();
			break;
		case MODE_SKY:
			mode_Sky_loop();
			break;
		case MODE_SPARKLES:
			mode_Sparkles_loop();
			break;
	}
}
