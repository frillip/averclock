void remote_init(void);
void remote_command(void);
void remote_feedback(void);

boolean led_status = 0;

#INT_RDA
void remote_command(void)
{
	char command = 0x00;
	command = fgetc(COM1);
	fprintf(COM1, "%c",command);
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

		case 'l':
			led_status = !led_status;
			output_bit(PIN_A1,led_status);
		break;

		case 'b':
			alarm = TRUE;
			manual_alarm=TRUE;
		break;

		case 'R':
			write_eeprom(EEPROM_RESET,0x42);			// Write reset flag
			write_eeprom(EEPROM_HOURS,time.hours);
			write_eeprom(EEPROM_MINUTES,time.minutes);
			write_eeprom(EEPROM_SECONDS,time.seconds);	// Write current time to EEPROM
			reset_cpu();
		break;
	}
	remote_feedback();
	update_display();
}

void remote_feedback(void)
{
	fprintf(COM1,"\r\n%02u:%02u:%02u - %u\r\n",time.hours,time.minutes,time.seconds,light);
}

