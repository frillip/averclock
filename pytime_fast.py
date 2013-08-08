#!/usr/bin/env python
# set the time manually using python. OMG HOW SILLY.
# could modify protocol to set time, but this is cool to watch.

import serial
import sys

from datetime import datetime

if len(sys.argv) == 1:
	print 'Usage ./pytime_fast.py <serial device>'
	sys.exit(1)

dev = sys.argv[1]

ser = serial.Serial(dev, 9600)

now = datetime.now()

# print time once (ask for twice for reliability)
ser.write('t')
ser.write('t')


time  = ser.readline()
hours, mins = time.split(':')
hours = int(hours)
mins  = int(mins)

# offset, number to increment by
d_hours = now.hour - hours
d_mins  = now.minute - mins

if d_hours > 0:
	ser.write('h' * d_hours)
elif d_hours < 0:
	ser.write('H' * abs(d_hours) )

if d_mins > 0:
	ser.write('m' * d_mins)
elif d_mins < 0:
	ser.write('M' * abs(d_mins) )

