averclock
=========

Just an exercise in timers. Oh, it's an CCS pic-compatable clock.

Forked from @naggie/averclock

Requires a [Sparkfun SPI serial display][1] with SS connected on PIN_B5 and
a watch crystal on the Timer1 oscillator pins. The PIC itself is clocked at 64MHz using
an external 16MHz crystal and the 4x PLL.

Time is automatically set from your computer's time when you compile the code and upload via PICKit,
or preserved in EEPROM if programming using the bootloader.

Connect an LDR on the low side of a potential divider (or an LED in reverse bias) to PIN_A0 for automatic
brightness control.

Bluetooth control
-----------------

Connect a TTL bluetooth serial port to the UART, or just a cable. Commands:

	m : increment minute
	M : decrement minute
	h : increment hour
	H : decrement hour
	l : flash LED
	any other character : print time

Use putty or picocom after pairing. From an android phone you can map buttons
to commands with the app 'Android bluetooth SPP'

[1]: http://proto-pic.co.uk/7-segment-serial-display-red/
