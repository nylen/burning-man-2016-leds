#define PIN_FLEX_SENSOR_0 A0

#define NUM_SENSORS 6
#define SENSOR_THRESHOLD 60

u8 hits[NUM_SENSORS];
u16 hitCenters[NUM_SENSORS];
u16 sensorValues[NUM_SENSORS];

u8 sensorConfiguration = 0;

#define ANIMATION_DISTANCE 15

void sensors_setup() {
	EEPROM.get(ADDRESS_SENSOR_CONFIG, sensorConfiguration);

	switch (sensorConfiguration) {
		case 1:
			hitCenters[0] = 25;
			hitCenters[1] = 75;
			hitCenters[2] = 125;
			// bit of a wiring mixup here...
			hitCenters[3] = 275;
			hitCenters[4] = 225;
			hitCenters[5] = 175;
			break;
		case 2:
			hitCenters[0] = 25;
			hitCenters[1] = 75;
			hitCenters[2] = 125;
			hitCenters[3] = 175;
			hitCenters[4] = 225;
			hitCenters[5] = 275;
			break;
		default:
			sensorConfiguration = 0;
			break;
	}

	for (u8 h = 0; h < NUM_SENSORS; h++) {
		hits[h] = 0;
		sensorValues[h] = SENSOR_THRESHOLD + 100;
	}
}

void sensors_loop(u16 frameMs) {
	if (!sensorConfiguration) {
		return;
	}

	// Try to keep each animation step the same length regardless of the
	// current frame length
	u8 framesPerStep = max(32 / frameMs, 1);

	// Perform hit animations
	for (u8 h = 0; h < NUM_SENSORS; h++) {
		if (hits[h] > 0) {
			u8 hitStep = hits[h] / framesPerStep;
			u8 extent = min(hitStep, ANIMATION_DISTANCE);
			for (u8 j = 0; j <= extent; j++) {
				if (j + ANIMATION_DISTANCE < hitStep) {
					// Skip the inside of the animation
					continue;
				}
				u8 alpha;
				CRGB hitColor;
				if (j + ANIMATION_DISTANCE - 5 < hitStep) {
					// 5 inner pixels of the receding edge
					alpha = (j + ANIMATION_DISTANCE - hitStep) * 40 + 55;
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
			if (++hits[h] == framesPerStep * ANIMATION_DISTANCE * 2) {
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
					hits[h] = framesPerStep;
				}
			}
		}
		sensorValues[h] = sensorValues[h] * 3 / 4 + value / 4;
	}
}

void command_setSensorConfiguration(bool hasValue, s16 value) {
	if (hasValue) {
		sensorConfiguration = (u8)value;

		EEPROM.put(ADDRESS_SENSOR_CONFIG, sensorConfiguration);
		
		// Rerun the initialization routine to set the correct sensor positions
		sensors_setup();
	}

	Serial.print("sensor config ");
	Serial.print(sensorConfiguration);
	Serial.println();
}
