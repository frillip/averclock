#!/usr/bin/env python
# set the time manually using python. OMG HOW SILLY.
# could modify protocol to set time, but this is cool to watch.

import serial
import sys

from datetime import datetime

if len(sys.argv) == 1:
	print 'Usage ./pytime.py <serial device>'
	sys.exit(1)

dev = sys.argv[1]

ser = serial.Serial(dev, 9600)

now = datetime.now()

# print time once
ser.write('t')

# set hours
while True:
	hours, mins = ser.readline().split(':')
	hours = int(hours)
	mins  = int(mins)

	if hours > now.hour:
		ser.write('H')
		continue
	elif hours < now.hour:
		ser.write('h')
		continue

	if mins > now.minute:
		ser.write('M')
		continue
	elif mins < now.minute:
		ser.write('m')
		continue

	if hours == now.hour and mins == now.minute:
		break
