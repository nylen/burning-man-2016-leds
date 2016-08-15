

void commands_loop() {
	if (!Serial.available()) {
		return;
	}

	u8 command = Serial.read();
	void (*commandPtr)(bool, s16) = NULL;
	switch (command) {
		case '-':
		case '+':
			commandPtr = &command_setDrift;
			break;
		case 'S':
		case 's':
			commandPtr = &command_setSensorConfiguration;
			break;
	}
	if (!commandPtr) {
		return;
	}

	s16 value = 0;
	bool isNegative = (command == '-');
	bool hasValue = false;

	while (true) {
		while (!Serial.available());
		u8 c = Serial.read();
		if (c < '0' || c > '9') {
			break;
		}
		hasValue = true;
		value = value * 10 + c - '0';
	}
	if (isNegative) {
		value = -value;
	}

	Serial.print((char)command);
	if (hasValue) {
		Serial.print(" (");
		Serial.print(value);
		Serial.print(")");
	}
	Serial.println();

	(*commandPtr)(hasValue, value);
}
