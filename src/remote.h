void remote_init(void);
void remote_command(void);
void remote_feedback(void);



void remote_init(void) {
	Serial.begin(9600);
}

void remote_command(void) {
	uint8_t command = 0x00;
	uint8_t i;
	if (Serial.available()) {
		command = Serial.read();
		switch (command) {
			case 'm':
				wallclock_inc_min();
			break;

			case 'M':
				wallclock_dec_min();
			break;

			case 'h':
				wallclock_inc_hour();
			break;

			case 'H':
				wallclock_dec_hour();
			break;


			//default:
			//	Serial.write(DEBUG_INVALID_COMMAND);
		}
		update_display();
		remote_feedback();
	}
}


void remote_feedback(void) {
	Serial.print(time.hours/10);
	Serial.print(time.hours%10);
	Serial.print(':');
	Serial.print(time.minutes/10);
	Serial.println(time.minutes%10);
}

