#include <EEPROM.h>

u16 t = 0;
u32 ms = 0;
u16 lastMinute = 0;
u16 currentMinute = 0;
s16 driftMsPerHour = 0;

void timing_setup() {
	EEPROM.get(ADDRESS_DRIFT, driftMsPerHour);

	// Save the runtime of the last boot

	// Copy 2 to 3
	EEPROM.get(ADDRESS_MINUTES_2, lastMinute);
	EEPROM.put(ADDRESS_MINUTES_3, lastMinute);

	// Copy 1 to 2
	EEPROM.get(ADDRESS_MINUTES_1, lastMinute);
	EEPROM.put(ADDRESS_MINUTES_2, lastMinute);

	// Copy 0 to 1
	EEPROM.get(ADDRESS_MINUTES_0, lastMinute);
	EEPROM.put(ADDRESS_MINUTES_1, lastMinute);

	// Reset current
	EEPROM.put(ADDRESS_MINUTES_0, currentMinute);
	lastMinute = 0;
}

void timing_loop() {
	ms = millis();
	ms += ((s32)(ms / 1000 / 60) * driftMsPerHour / 60);
	t = ms / 16;

	currentMinute = ms / 1000 / 60;
	if (currentMinute != lastMinute) {
		if (currentMinute % 5 == 0) {
			EEPROM.put(ADDRESS_MINUTES_0, currentMinute);
		}
		lastMinute = currentMinute;
	}

	s16 serialByte = Serial.read();
	if (serialByte != '-' && serialByte != '+') {
		return;
	}

	delayMicroseconds(80); // wait for next byte

	bool isNegative = (serialByte == '-');
	u16 drift = 0;
	while ((serialByte = Serial.read()) > -1) {
		if (serialByte < '0' || serialByte > '9') {
			break;
		}
		drift = drift * 10 + serialByte - '0';
		delayMicroseconds(80);
	}

	u16 previousBootMinutes;

	EEPROM.get(ADDRESS_MINUTES_3, previousBootMinutes);
	Serial.print("last-2 minutes: ");
	Serial.print(previousBootMinutes);
	Serial.println();

	EEPROM.get(ADDRESS_MINUTES_2, previousBootMinutes);
	Serial.print("last-1 minutes: ");
	Serial.print(previousBootMinutes);
	Serial.println();

	EEPROM.get(ADDRESS_MINUTES_1, previousBootMinutes);
	Serial.print("last minutes: ");
	Serial.print(previousBootMinutes);
	Serial.println();

	Serial.print("minutes active: ");
	Serial.print(currentMinute);
	Serial.println();

	Serial.print("drift was ");
	Serial.print(driftMsPerHour);
	Serial.print("ms/hr");

	if (drift > 0) {
		driftMsPerHour = isNegative ? -drift : drift;
		EEPROM.put(ADDRESS_DRIFT, driftMsPerHour);
	}

	Serial.print(" now ");
	Serial.print(driftMsPerHour);
	Serial.print("ms/hr");
	Serial.println();
}
