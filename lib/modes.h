#define MS_PER_MODE_STEP (10 /* * 60 */ * 1000)
u8 currentModeStep = 0;
u8 NUM_MODE_STEPS = 1; // calculated in setup()

#include "mode_Snakes.h"
#define MODE_SNAKES 0
#include "mode_Sky.h"
#define MODE_SKY 1

#define NUM_MODES 2

u8 MODE_STEPS[] = {
	MODE_SKY, MODE_SKY,
	MODE_SNAKES,
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

	switch (MODE_STEPS[currentModeStep]) {
		case MODE_SNAKES:
			mode_Snakes_loop();
			break;
		case MODE_SKY:
			mode_Sky_loop();
			break;
	}
}
