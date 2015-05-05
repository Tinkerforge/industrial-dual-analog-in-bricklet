#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call industrial-dual-analog-in-bricklet $uid set-debounce-period 10000

# configure threshold (channel 1) for "greater than 5V" (unit is mV)
tinkerforge call industrial-dual-analog-in-bricklet $uid set-voltage-callback-threshold 1 greater 5000 0

# handle incoming voltage-reached callbacks (unit is mV)
tinkerforge dispatch industrial-dual-analog-in-bricklet $uid voltage-reached\
 --execute "echo Voltage for channel {channel} is greater than 5V: {voltage} mV"
