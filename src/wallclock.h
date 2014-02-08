#define EEPROM_RESET	0x00
#define EEPROM_HOURS	0x01
#define EEPROM_MINUTES	0x02
#define EEPROM_SECONDS	0x03
#IFDEF DRINKING_GAME
#define EEPROM_SHOTS	0x10
#ENDIF
#define ALARM_PIN		PIN_C2
#define ALARM_DURATION	3

void wallclock_inc_sec (void);
void wallclock_dec_sec (void);
void wallclock_inc_min (void);
void wallclock_dec_min (void);
void wallclock_inc_hour (void);
void wallclock_dec_hour (void);
void wallclock_alarm(void);

typedef struct {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
} elapsed;

elapsed time = {0,0,0};

boolean alarm = FALSE;
boolean manual_alarm = FALSE;
uint8_t alarm_count = 0;

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
#IFDEF DRINKING_GAME
	if((shot_count<100)&&(!shots_finished))
	{
		shot_count++;
		alarm=TRUE;
		manual_alarm=TRUE;
		alarm_count=0;
		fprintf(COM1,"%u\r\n", shot_count);
	}
	if((shot_count==100)&&(!shots_finished))
	{
		fprintf(COM1,"ALL DONE!\r\n");
		alarm=TRUE;
	}
#ENDIF
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

void wallclock_alarm(void)
{
	alarm=0;
	if(time.seconds<ALARM_DURATION)
	{
		if((time.hours==9)&&(time.minutes==20))
		{
			alarm = TRUE;
		}
		else if((time.hours==12)&&(time.minutes==40))
		{
			alarm = TRUE;
		}
		else if((time.hours==14)&&(time.minutes==55))
		{
			alarm = TRUE;
		}
		else if((time.hours==16)&&(time.minutes==50))
		{
			alarm = TRUE;
		}
		else alarm = FALSE;
	}
	else alarm = FALSE;
}