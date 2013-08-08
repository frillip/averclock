void init_display(void);
void update_display (char);
void update_brightness (void);
void toggle_colon(void);

#define DISP_BRIGHTEST 0x00
boolean colon_state=FALSE;

void update_display ()
{
	output_low(DISP_SS);
	
	// hours (1-2 digit)
	spi_write(time.hours/10);
	spi_write(time.hours%10);

	// minutes
	spi_write(time.minutes/10);
	spi_write(time.minutes%10);

	// deselect display
	output_high(DISP_SS);
}

void init_display(void)
{
	// Set SS high
	output_high(DISP_SS);
	// SPI at 250kHz
	setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_T2);
	// wait for it to boot
	delay_ms(300);

	// reset, turn on colon
	output_low(DISP_SS);
	// reset
	spi_write(0x76);
	// dots
	spi_write(0x77);
	// colon
	spi_write(0x10);
	// max brightness
	spi_write(0x7A);
	spi_write(DISP_BRIGHTEST);
	output_high(DISP_SS);

	// fill with initial time (force)
	update_display();
}

/*
void update_brightness() {
	unsigned int light = 0;

	// is display bright?
	static bool bright = 1;

	light = analogRead(LDR_PIN);

	// room is bright
	if ((light < BRIGHTNESS_THRESH_LIGHT) && !bright) {
		digitalWrite(DISP_SS,0);
		// max brightness
		spi_write(0x7A);
		spi_write(DISP_BRIGHTEST);
		bright = 1;
		digitalWrite(DISP_SS,1);
	// room is dark
	} else if ((light > BRIGHTNESS_THRESH_DARK) && bright) {
		digitalWrite(DISP_SS,0);
		// dim display
		spi_write(0x7A);
		spi_write(DISP_DIMMEST);
		bright = 0;
		// deselect display
		digitalWrite(DISP_SS,1);
	}
}
*/

/*
void display_adc() {
	unsigned int light = 0;

	light = analogRead(LDR_PIN);

	digitalWrite(DISP_SS,0);

	spi_write(light/1000%10);
	spi_write(light/100 %10);
	spi_write(light/10  %10);
	spi_write(light/1   %10);

	// deselect display
	digitalWrite(DISP_SS,1);
}
*/

void toggle_colon(void)
{
	colon_state = ! colon_state;
	output_low(DISP_SS);
	// dots
	spi_write(0x77);
	// colon or no colon
	spi_write((uint8_t)colon_state<<4);
	// deselect display
	output_high(DISP_SS);
}

