#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_industrial_dual_analog_in import IndustrialDualAnalogIn

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    dual_ain = IndustrialDualAnalogIn(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current voltage for channel 0 (unit is mV)
    voltage = dual_ain.get_voltage(0)
    print('Voltage: ' + str(voltage/1000.0) + ' V')

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()