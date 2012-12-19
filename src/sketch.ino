// TODO general scheduler addTask(pointer,interval,offset)
// and better with a queue and priorities?

// TODO feed-forward brightness adjusting based on LDR value. 2 values is fine.
// TODO debounced (async) buttons for setting time
// TODO alarm?
// TODO split into headers (and defines for SPI commands)
// TODO use registers instead of digitalwrite, etc

#include "current_time.h"
#include <SPI.h>

// tick interval in ms, 1-20 recommended, 260 max
#define TICK 20

// 1 second intervals to update clock
#define HEARTBEAT_PERIOD (1000/TICK)

// display SPI slave select pin
#define DISP_SS 10

volatile int heartbeat_count = 0;


typedef struct {
	char hours;
	char minutes;
	char seconds;
} elapsed;

// initialise with compile-time time with an offset to account for buildi/upload time
elapsed time = {HOURS,MINUTES,SECONDS+4};

void init_display(void);

// tasks
void inc_time (void);
void update_display (void);


void setup () {
	// enable interrupts (match A, too)
	noInterrupts();
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1  = 0;

	// set up timer1 @ 16MHz to call ISR every TICK ms
	// will clobber servo lib and pwm 9 and 10

	// TODO: A and B or what????

	// SYS clock @ 16Mhz, Prescaler at 64, 16 bit timer1
	// so 250 per ms
	OCR1A = 250*TICK;

	// CTC mode (clear timer on compare match
	TCCR1B |= (1 << WGM12);

	// 64 prescaler
	TCCR1B |= (1 << CS11);
	TCCR1B |= (1 << CS10);

	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);

	interrupts();

	init_display();
}

void loop () {
	// standby mode with timer1 active and CPU off
	SMCR |= 0b1101;
}

// Interrupt service routine, scheduler
ISR(TIMER1_COMPA_vect) {
	if (heartbeat_count++ == HEARTBEAT_PERIOD) {
		heartbeat_count = 0;
		inc_time();
	}

	// TODO: reset interrupt flag!
}

// call at 1Hz
void inc_time (void) {
	if (time.seconds++ == 60) {
		time.minutes++;
		time.seconds = 0;
	}

	if (time.minutes++ == 60) {
		time.hours++;
		time.minutes = 0;
	}

	if (time.hours == 24)
		time.hours = 0;
}

void update_display (void) {
	// select dsiplay
	digitalWrite(DISP_SS,0);

	// reset display, just in case the cursor has been moved
	SPI.transfer(0x77);

	// hours
	SPI.transfer(time.hours%10);
	SPI.transfer(time.hours/10);

	// minutes
	SPI.transfer(time.minutes%10);
	SPI.transfer(time.minutes/10);

	// deselect display
	digitalWrite(DISP_SS,1);
}

void init_display(void) {
	// init SPI

	// slave select pin init
	pinMode (DISP_SS, OUTPUT);
	digitalWrite(DISP_SS,1);

	// initialize SPI:
	SPI.begin();

	// turn on colon
	SPI.transfer(0x77);
	SPI.transfer(0x08);

	// fill with initial time
	update_display();
}
