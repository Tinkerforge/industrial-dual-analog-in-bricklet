#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Dual Analog In Bricklet

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call industrial-dual-analog-in-bricklet $uid set-debounce-period 10000

# Handle incoming voltage reached callbacks (parameter has unit mV)
tinkerforge dispatch industrial-dual-analog-in-bricklet $uid voltage-reached &

# Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
tinkerforge call industrial-dual-analog-in-bricklet $uid set-voltage-callback-threshold 1 threshold-option-greater 10000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
