#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get current voltage from channel 1 (unit is mV)
tinkerforge call industrial-dual-analog-in-bricklet $uid get-voltage 1
