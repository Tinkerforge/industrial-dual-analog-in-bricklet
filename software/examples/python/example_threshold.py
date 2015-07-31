#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_dual_analog_in import IndustrialDualAnalogIn

# Callback for voltage greater than 5V
def cb_reached(channel, voltage):
    print('Voltage (Channel' + str(channel) + ') is greater than 5V: ' + str(voltage/1000.0))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    idai = IndustrialDualAnalogIn(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    idai.set_debounce_period(10000)

    # Register threshold reached callback to function cb_reached
    idai.register_callback(idai.CALLBACK_VOLTAGE_REACHED, cb_reached)

    # Configure threshold (channel 1) for "greater than 5V" (unit is mV)
    idai.set_voltage_callback_threshold(1, '>', 5*1000, 0)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
