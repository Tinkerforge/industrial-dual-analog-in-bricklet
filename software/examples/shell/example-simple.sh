#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Dual Analog In Bricklet

# Get current voltage from channel 1
tinkerforge call industrial-dual-analog-in-bricklet $uid get-voltage 1
