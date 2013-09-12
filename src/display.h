void init_display(void);
void update_display (char);
void update_brightness (void);
void toggle_colon(void);

#define DISP_BRIGHTEST 0x00
#define DISP_DIMMEST 0x40
#define LDR_BRIGHTEST 60
#define LDR_DIMMEST	50

uint8_t disp_brightness=0x00;
boolean colon_state=FALSE;
uint8_t light=0;
uint8_t last_light=0;

void update_display ()
{
	output_low(DISP_SS);
	#asm nop #endasm

	// hours (1-2 digit)
	spi_write(time.hours/10);
	spi_write(time.hours%10);

	// minutes
	spi_write(time.minutes/10);
	spi_write(time.minutes%10);

	// deselect display
	#asm nop #endasm
	output_high(DISP_SS);
}

void init_display(void)
{
	// Set SS high
	output_high(DISP_SS);
	// SPI at 250kHz
	setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_XMIT_L_TO_H|SPI_CLK_T2);
	// wait for it to boot
	delay_ms(100);

	// reset, turn on colon
	output_low(DISP_SS);
	#asm nop #endasm
	// reset
	spi_write(0x76);
	// dots
	spi_write(0x77);
	// colon
	spi_write(0x10);
	// max brightness
	spi_write(0x7A);
	spi_write(DISP_BRIGHTEST);
	#asm nop #endasm
	output_high(DISP_SS);

	// fill with initial time (force)
	update_display();
}

void update_brightness() {

	// fancy automagical scaling
	last_light=light;
	light = read_adc();
	if(light!=last_light)
	{
		if(light>LDR_BRIGHTEST) light=LDR_BRIGHTEST;
		if(light<LDR_DIMMEST) light=LDR_DIMMEST;
		disp_brightness=(60-light)*2;
		output_low(DISP_SS);
		#asm nop #endasm
		spi_write(0x7A);
		spi_write(disp_brightness);
		#asm nop #endasm
		output_high(DISP_SS);
	}
}

void toggle_colon(void)
{
	colon_state = ! colon_state;
	output_low(DISP_SS);
	#asm nop #endasm
	// dots
	spi_write(0x77);
	// colon or no colon
	spi_write((uint8_t)colon_state<<4);
	// deselect display
	#asm nop #endasm
	output_high(DISP_SS);
}

