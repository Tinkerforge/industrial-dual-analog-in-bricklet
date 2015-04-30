#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_dual_analog_in import IndustrialDualAnalogIn

# Callback for voltage greater than 10V
def cb_reached(channel, voltage):
    print('Voltage (channel ' + str(channel) + ') is greater than 10V: ' + str(voltage/1000.0))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    dual_ain = IndustrialDualAnalogIn(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    dual_ain.set_debounce_period(10000)

    # Register threshold reached callback to function cb_reached
    dual_ain.register_callback(dual_ain.CALLBACK_VOLTAGE_REACHED, cb_reached)

    # Configure threshold (channel 0) for "greater than 10V" (unit is mV)
    dual_ain.set_voltage_callback_threshold(0, '>', 10*1000, 0)

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
