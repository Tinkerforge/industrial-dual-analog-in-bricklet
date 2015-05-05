#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ

# get current voltage from channel 1 (unit is mV)
tinkerforge call industrial-dual-analog-in-bricklet $uid get-voltage 1
