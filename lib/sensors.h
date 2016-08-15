#define PIN_FLEX_SENSOR_0 A0

#define NUM_SENSORS 6
#define SENSOR_THRESHOLD 60

u8 hits[NUM_SENSORS];
u16 hitCenters[NUM_SENSORS];
u16 sensorValues[NUM_SENSORS];

#define ANIMATION_DISTANCE 15

void sensors_setup() {
	hitCenters[0] = 25;
	hitCenters[1] = 75;
	hitCenters[2] = 125;
	// bit of a wiring mixup here...
	hitCenters[3] = 275;
	hitCenters[4] = 225;
	hitCenters[5] = 175;
	for (u8 h = 0; h < NUM_SENSORS; h++) {
		hits[h] = 0;
		sensorValues[h] = SENSOR_THRESHOLD + 100;
	}
}

void sensors_loop(u16 frameMs) {
	// Perform hit animations
	for (u8 h = 0; h < NUM_SENSORS; h++) {
		if (hits[h] > 0) {
			u8 extent = min(hits[h], ANIMATION_DISTANCE);
			for (u8 j = 0; j <= extent; j++) {
				if (j + ANIMATION_DISTANCE < hits[h]) {
					// Skip the inside of the animation
					continue;
				}
				u8 alpha;
				CRGB hitColor;
				if (j + ANIMATION_DISTANCE - 5 < hits[h]) {
					// 5 inner pixels of the receding edge
					alpha = (j + ANIMATION_DISTANCE - hits[h]) * 40 + 55;
					hitColor = blend(yellow, orange, alpha);
				} else if (j == extent) {
					// Outer edge
					alpha = 150;
					hitColor = yellow;
				} else if (j == extent - 1) {
					// Next to outer edge
					alpha = 200;
					hitColor = orange;
				} else if (j == extent - 2 || j == extent - 3) {
					alpha = 255;
					hitColor = orange;
				} else {
					alpha = 255;
					hitColor = red;
				}
				if (hitCenters[h] + j < NUM_LEDS) {
					setLed(
						hitCenters[h] + j,
						blend(
							getLed(hitCenters[h] + j),
							hitColor,
							alpha
						)
					);
				}
				if (hitCenters[h] >= j) {
					setLed(
						hitCenters[h] - j,
						blend(
							getLed(hitCenters[h] - j),
							hitColor,
							alpha
						)
					);
				}
			}
			//delay(50); // debugging the animation
			if (++hits[h] == ANIMATION_DISTANCE * 2) {
				hits[h] = 0;
			}
		}
	}

	// Read flex sensors
	for (u8 h = 0; h < NUM_SENSORS; h++) {
		u16 value = analogRead(PIN_FLEX_SENSOR_0 + h);
		if (value >= 1000) {
			// Probably a bad measurement; skip it
			continue;
		}
		if (value > sensorValues[h] + SENSOR_THRESHOLD && hits[h] == 0) {
			// Read again to eliminate some noise (EMI?)
			value = analogRead(PIN_FLEX_SENSOR_0 + h);
			if (value > sensorValues[h] + SENSOR_THRESHOLD) {
				value = analogRead(PIN_FLEX_SENSOR_0 + h);
				if (value > sensorValues[h] + SENSOR_THRESHOLD) {
					hits[h] = 1;
				}
			}
		}
		sensorValues[h] = sensorValues[h] * 3 / 4 + value / 4;
	}
}
