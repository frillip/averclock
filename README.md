averclock
=========

Just an exercise in timers. Oh, it's a CCS pic-compatable clock.

Forked from [@naggie/averclock][5]

Requires a [Sparkfun SPI serial display][1] with SS connected on PIN_B5 and
a watch crystal on the Timer1 oscillator pins. The PIC itself is clocked at 64MHz using
an external 16MHz crystal and the internal 4x PLL.

Time is automatically set from your computer's time when you compile the code and upload via PICKit,
or preserved in EEPROM if programming using the bootloader.

Connect an LDR on the low side of a potential divider (or an LED in reverse bias) to PIN_A0
for automatic brightness control.

Connect a self oscillating buzzer on PIN_C2 for noise at semi regular intervals. This is also a
PWM pin, so you could also use a piezio speaker with minimal code modification.

Bluetooth control
-----------------

Connect a TTL bluetooth serial port to the UART, or just a cable, configure for 115200n1. The code
supports the following AT command set:

	AT+SECONDS	: Reset seconds to 0
	AT+MINUTES+	: Increment minute
	AT+MINUTES-	: Decrement minute
	AT+HOURS+	: Increment hour
	AT+HOURS-	: Decrement hour
	AT+LEDx		: Set LED state, x=0 off, x=1 on, toggle if x is absent
	AT+TIME		: Print current time
	AT+TIMExxyyzz	: Set time to xx:yy:zz, if zz is absent, time will be set to xx:yy:00
	AT+ALARM	: Beep alarm
	AT+RESET	: Reset the device

Use putty or picocom after pairing. From an android phone you can map buttons
to commands with the Android app '[Bluetooth SPP Pro][4]'

Programming over bluetooth
--------------------------

I have modified a version of [tinybld][2] to work with the PIC18F26K80.
This allows you to program over serial. In this instance, we are using a bluetooth to TTL
converter instead of a cable/level converter. Compile the bootloader using the MPASM toolsuite
and flash using the PICKit. Simply pair the devices and configure the serial port appropriately.

Acquire the latest bootloader PC application from [here][3] and configure it to send '0xff'
over serial first and ignore any additional crud from the PIC, then write flash.

[1]: http://proto-pic.co.uk/7-segment-serial-display-red/
[2]: http://www.etc.ugal.ro/cchiculita/software/picbootloader.htm
[3]: http://www.etc.ugal.ro/cchiculita/software/tinyblddownload.htm
[4]: https://play.google.com/store/apps/details?id=mobi.dzs.android.BLE_SPP_PRO&hl=en_GB
[5]: https://github.com/naggie/averclock/
