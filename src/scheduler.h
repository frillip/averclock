uint8_t t1s0=0;

#INT_TIMER1
void second(void)
{
	set_timer1(-32768);	// Reset the timer, assuming 32.768Hz crystal
	t1s0++;
}