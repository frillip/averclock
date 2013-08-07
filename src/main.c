#include "18F2685.h"
#device ADC=11
#fuses NOWDT,NOPROTECT,NOLVP,INTRC_IO
#use delay(clock=16000000)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,ERRORS,stream=COM1)

#define DISP_SS PIN_B4

#include "stdint.h"				// Standard int types
#include "scheduler.h"
#include "wallclock.h"
#include "display.h"
#include "remote.h"

char timestr[9]=__TIME__;

void main(void)
{
	setup_oscillator(OSC_16MHZ);
	set_tris_b(0x09);
	set_tris_c(0x93);
	setup_timer_1(T1_EXTERNAL | 0x8);		// Set up the scheduler timer
	enable_interrupts(INT_TIMER1);	// Enable scheduler timer interrupt
	enable_interrupts(GLOBAL);		// Enable interrupts globally

	time.seconds=(((uint8_t)timestr[6]-48)*10)+((uint8_t)timestr[7]-45);
	time.minutes=(((uint8_t)timestr[3]-48)*10)+((uint8_t)timestr[4]-48);
	time.hours=(((uint8_t)timestr[0]-48)*10)+((uint8_t)timestr[1]-48);
	
	init_display();

	t1s0=0;
	set_timer1(-32768);
	while(TRUE)
	{	
		if(t1s0==1)
		{
			t1s0=0;
			toggle_colon();
			wallclock_inc_sec();
			update_display();
		}
		if(kbhit(COM1))
			remote_command();
	}
}