uint8_t t10ms=0;
uint8_t t10ms0=0;
uint8_t t100ms=0;
uint8_t t100ms0=0;
uint8_t t100ms1=0;
uint8_t t1s0=0;

#INT_TIMER1
void timekeeping(void)
{
	set_timer1(-32768);	// Reset the timer, assuming 32.768Hz crystal
	output_toggle(PIN_B4);
	wallclock_inc_sec();
}

#INT_TIMER3
void scheduler(void)
{
	set_timer3(-20000);
	t10ms++;
	t10ms0++;
	if(t10ms==10)
	{
		t10ms=0;
		t100ms++;
		t100ms0++;
		t100ms1++;
		if(t100ms==10)
		{
			t100ms=0;
			t1s0++;
		}
	}
}
