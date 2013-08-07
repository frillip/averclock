averclock
=========

Just an exercise in timers. Oh, it's an CCS pic-compatable clock.

Forked from @naggie/averclock

Requires a [Sparkfun SPI serial display][1] with SS connected on PIN_B4 and
a watch crystal on the Timer1 oscillator pins. The PIC itself is clocked at 16MHz
using the internal oscillator.

Time is automatically set from your computer's time when you compile the code.

Connect an LDR on the high side of a potential divider to PIN_A0 for automatic
brightness control.

Bluetooth control
-----------------

Connect a TTL bluetooth serial port to the UART, or just a cable. Commands:

	m : increment minute
	M : decrement minute
	h : increment hour
	H : decrement hour

Use putty or picocom after pairing. From an android phone you can map buttons
to commands with the app 'Android bluetooth SPP'

[1]: http://proto-pic.co.uk/7-segment-serial-display-red/
