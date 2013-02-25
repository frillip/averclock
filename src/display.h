#include <SPI.h>

void init_display(void);
void update_display (char);
void update_brightness (void);
void toggle_colon(void);

void update_display () {
	static char colon_state = 1;

	colon_state = ! colon_state;

	digitalWrite(DISP_SS,0);

	// hours (1-2 digit)
	SPI.transfer(time.hours/10?:'x');
	SPI.transfer(time.hours%10);

	// minutes
	SPI.transfer(time.minutes/10);
	SPI.transfer(time.minutes%10);

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
	SPI.setClockDivider(SPI_CLOCK_DIV64);

	// wait for it to boot
	delay(300);

	// reset, turn on colon
	digitalWrite(DISP_SS,0);
	// reset
	SPI.transfer(0x76);
	// dots
	SPI.transfer(0x77);
	// colon
	SPI.transfer(0x10);
	// max brightness
	SPI.transfer(0x7A);
	SPI.transfer(DISP_BRIGHTEST);
	digitalWrite(DISP_SS,1);

	// fill with initial time (force)
	update_display();
}

void update_brightness() {
	unsigned int light = 0;

	// is display bright?
	static bool bright = 1;

	light = analogRead(LDR_PIN);

	// room is bright
	if ((light < BRIGHTNESS_THRESH_LIGHT) && !bright) {
		digitalWrite(DISP_SS,0);
		// max brightness
		SPI.transfer(0x7A);
		SPI.transfer(DISP_BRIGHTEST);
		bright = 1;
		digitalWrite(DISP_SS,1);
	// room is dark
	} else if ((light > BRIGHTNESS_THRESH_DARK) && bright) {
		digitalWrite(DISP_SS,0);
		// dim display
		SPI.transfer(0x7A);
		SPI.transfer(DISP_DIMMEST);
		bright = 0;
		// deselect display
		digitalWrite(DISP_SS,1);
	}

}

void display_adc() {
	unsigned int light = 0;

	light = analogRead(LDR_PIN);

	digitalWrite(DISP_SS,0);

	SPI.transfer(light/1000%10);
	SPI.transfer(light/100 %10);
	SPI.transfer(light/10  %10);
	SPI.transfer(light/1   %10);

	// deselect display
	digitalWrite(DISP_SS,1);
}

void toggle_colon(void) {
	static bool colon_state = 1;

	colon_state = ! colon_state;

	digitalWrite(DISP_SS,0);

	// dots
	SPI.transfer(0x77);
	// colon or no colon
	SPI.transfer(colon_state?1<<4:0);

	// deselect display
	digitalWrite(DISP_SS,1);
}
