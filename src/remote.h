void remote_init(void);
void remote_command(void);
void remote_feedback(void);

void remote_command(void)
{
	static boolean led_status = 1;
	char command = 0x00;
	command = getc();
	//fprintf(COM1, "%c",command);
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
			output_bit(PIN_A5,led_status);
		break;

		case 'p':
			remote_feedback();
		break;
	}
	update_display();
}

void remote_feedback(void) {
	printf("%02u%02u",time.hours,time.minutes);
}

