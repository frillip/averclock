averclock
=========

Just an exercise in timers. Oh, it's an CCS pic-compatable clock.

Forked from @naggie/averclock

Requires a [Sparkfun SPI serial display][1] with SS connected on PIN_B5 and
a watch crystal on the Timer1 oscillator pins. The PIC itself is clocked at 64MHz using
an external 16MHz crystal and the 4x PLL.

Time is automatically set from your computer's time when you compile the code and upload via PICKit,
or preserved in EEPROM if programming using the bootloader.

Connect an LDR on the low side of a potential divider (or an LED in reverse bias) to PIN_A0
for automatic brightness control.

Connect a self oscillating buzzer on PIN_C2 for noise at semi regular intervals. This is also a
PWM pin, so you could also use a piezio speaker with minimal code modification.

Bluetooth control
-----------------

Connect a TTL bluetooth serial port to the UART, or just a cable, configure for 115200n1. Commands:

	m : increment minute
	M : decrement minute
	h : increment hour
	H : decrement hour
	l : flash LED
	b : beep alarm
	R : reset the device
	any other character : print time and LDR level

Use putty or picocom after pairing. From an android phone you can map buttons
to commands with the app 'Android bluetooth SPP'

Programming over bluetooth
--------------------------

There is a version of tinybld that has been modified to work with the PIC18F26K80.
This allows you to program over serial. In this instance, we are using a bluetooth to TTL
converter. Compile the bootloader using the MPASM toolsuite and flash using the PICKit 3.
Simply pair the devices and configure the serial port appropriately.

Acquire the latest bootlaoder PC application from [here][2] and configure it to send 'R'
over serial first and ignore any additional crud from the PIC, then write flash.

[1]: http://proto-pic.co.uk/7-segment-serial-display-red/
[2]: http://www.etc.ugal.ro/cchiculita/software/tinyblddownload.htm
