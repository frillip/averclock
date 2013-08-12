#include "18F2685.h"
#fuses NOWDT,NOPROTECT,NOLVP,INTRC_IO
#use delay(clock=32000000)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,ERRORS,stream=COM1)

#define DISP_SS PIN_B5

#include "stdint.h"				// Standard int types
#include "wallclock.h"
#include "scheduler.h"
#include "display.h"
#include "remote.h"

char timestr[9]=__TIME__;

void main(void)
{
	setup_oscillator(OSC_32MHZ);
	setup_adc(ADC_CLOCK_DIV_8);
	setup_adc_ports(AN0);
	set_adc_channel(0);
	setup_timer_1(T1_EXTERNAL | T1_CLK_OUT);	// Set up the timekeeping timer
	setup_timer_2(T2_DIV_BY_1, 0x10, 1);		// Set up SPI clock timer
	setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);	// Set up scheduler timer
	enable_interrupts(INT_RDA);					// Enable serial interrupt
	enable_interrupts(INT_TIMER1);				// Enable timekeeping timer interrupt
	enable_interrupts(INT_TIMER3);				// Enable scheduler timer interrupt
	enable_interrupts(GLOBAL);					// Enable interrupts globally

	time.seconds=(((uint8_t)timestr[6]-48)*10)+((uint8_t)timestr[7]-46);
	time.minutes=(((uint8_t)timestr[3]-48)*10)+((uint8_t)timestr[4]-48);
	time.hours=(((uint8_t)timestr[0]-48)*10)+((uint8_t)timestr[1]-48);		// Parse timestr to time struct
	
	init_display();

	set_timer1(-32768);		// Begin timekeeping

	t10ms=0;
	t100ms=0;
	t100ms0=0;
	t1s0=0;
	set_timer3(-10000);		// Reset and set scheduler
	while(TRUE)
	{
		if(t10ms0==1)
		{
			output_toggle(PIN_C2);
			t10ms0=0;
		}
		if(t100ms0==1)
		{
			t100ms0=0;
			update_brightness();
		}
		if(t1s0==1)
		{
			t1s0=0;
			toggle_colon();
			update_display();
		}
	}
}
