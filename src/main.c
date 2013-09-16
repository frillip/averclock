#include "18F26K80.h"
#device PASS_STRINGS = IN_RAM
#fuses NOPROTECT,SOSC_LOW,HSH,PLLEN,WDT,WDT128
#use delay(clock=64000000,crystal=16000000,restart_wdt)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,ERRORS,stream=COM1,restart_wdt)	// TTL serial for bluetooth etc
#use rs232(baud=115200,parity=N,xmit=PIN_B6,rcv=PIN_B7,bits=8,ERRORS,stream=COM2,restart_wdt)	// TTL serial for GPS

#define DISP_SS PIN_B5

#include "stdint.h"				// Standard int types
#include "wallclock.h"
#include "scheduler.h"
#include "display.h"
#include "remote.h"

char timestr[9]=__TIME__;

void main(void)
{
	setup_adc(ADC_CLOCK_DIV_8);
	setup_adc_ports(sAN0);
	set_adc_channel(0);
	setup_wdt(WDT_ON);
	setup_timer_1(T1_EXTERNAL | T1_ENABLE_SOSC);	// Set up the timekeeping timer
	setup_timer_2(T2_DIV_BY_1, 0x28, 1);		// Set up SPI clock timer
	setup_timer_3(T3_INTERNAL | T3_DIV_BY_8);	// Set up scheduler timer
	output_low(ALARM_PIN);
	enable_interrupts(INT_RDA);					// Enable serial interrupt
	enable_interrupts(INT_TIMER1);				// Enable timekeeping timer interrupt
	enable_interrupts(INT_TIMER3);				// Enable scheduler timer interrupt
	enable_interrupts(GLOBAL);					// Enable interrupts globally

	if(read_eeprom(EEPROM_RESET)==0x42)
	{
		time.hours=read_eeprom(EEPROM_HOURS);
		time.minutes=read_eeprom(EEPROM_MINUTES);
		time.seconds=read_eeprom(EEPROM_SECONDS);
		write_eeprom(EEPROM_RESET,0x00);
	}
	else
	{
		time.seconds=(((uint8_t)timestr[6]-48)*10)+((uint8_t)timestr[7]-46);
		time.minutes=(((uint8_t)timestr[3]-48)*10)+((uint8_t)timestr[4]-48);
		time.hours=(((uint8_t)timestr[0]-48)*10)+((uint8_t)timestr[1]-48);		// Parse timestr to time struct
	}
	memset(command_buffer, 0, sizeof(command_buffer));
	memset(command, 0, sizeof(command));
	
	restart_wdt();
	init_display();

	fprintf(COM1, "HELLO!\r\n");	// Say hello!

	set_timer1(-32768);			// Begin timekeeping

	t10ms=0;
	t100ms=0;
	t100ms0=0;
	t1s0=0;
	set_timer3(-20000);			// Reset and set scheduler
	while(TRUE)
	{
		restart_wdt();
		if(t10ms0==1)
		{
			t10ms0=0;
			if(command_waiting) process_command();
		}
		if(t100ms0==1)
		{
			t100ms0=0;
			update_brightness();
			if((alarm)&&(alarm_count<5))
			{
				if(alarm_count==0) output_high(ALARM_PIN);
				alarm_count++;	
			}
			else
			{
				output_low(ALARM_PIN);
				alarm=FALSE;
				alarm_count=0;
			}
		}
		if(t100ms1==5)
		{
			t100ms1=0;
		}
		if(t1s0==1)
		{
			t1s0=0;
			toggle_colon();
			update_display();
			if(manual_alarm==FALSE) wallclock_alarm();
		}
	}
}
