#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# set period for voltage callback to 1s (1000ms)
# note: the voltage callback is only called every second if the
#       voltage has changed since the last call!
tinkerforge call industrial-dual-analog-in-bricklet $uid set-voltage-callback-period 1 1000

# handle incoming callbacks (unit is mV)
tinkerforge dispatch industrial-dual-analog-in-bricklet $uid voltage
