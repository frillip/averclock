void wallclock_inc_sec (void);
void wallclock_dec_sec (void);
void wallclock_inc_min (void);
void wallclock_dec_min (void);
void wallclock_inc_hour (void);
void wallclock_dec_hour (void);

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} elapsed;

elapsed time = {0,0,0};

// call at 1Hz
void wallclock_inc_sec (void)
{
	time.seconds++;
	if (time.seconds == 60) {
		wallclock_inc_min();
		time.seconds = 0;
	}
}

void wallclock_inc_min (void)
{
	time.minutes++;
	if (time.minutes == 60) {
		wallclock_inc_hour();
		time.minutes = 0;
	}
}
void wallclock_inc_hour (void)
{
	time.hours++;
	if (time.hours == 24)
		time.hours = 0;
}


void wallclock_dec_sec(void)
{
	time.seconds--;
	if (time.seconds == 255) {
		wallclock_dec_min();
		time.seconds = 59;
	}
}

void wallclock_dec_min (void)
{
	time.minutes--;
	if (time.minutes == 255) {
		wallclock_dec_hour();
		time.minutes = 59;
	}
}

void wallclock_dec_hour (void)
{
	time.hours--;
	if (time.hours ==255)
		time.hours = 23;
}
