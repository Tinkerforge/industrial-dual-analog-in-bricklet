#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Industrial Dual Analog In Bricklet

# Handle incoming voltage callbacks (parameter has unit mV)
tinkerforge dispatch industrial-dual-analog-in-bricklet $uid voltage &

# Set period for voltage (channel 1) callback to 1s (1000ms)
# Note: The voltage (channel 1) callback is only called every second
#       if the voltage (channel 1) has changed since the last call!
tinkerforge call industrial-dual-analog-in-bricklet $uid set-voltage-callback-period 1 1000

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
