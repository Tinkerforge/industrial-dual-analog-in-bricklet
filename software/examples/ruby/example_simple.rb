#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_dual_analog_in'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
idai = BrickletIndustrialDualAnalogIn.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current voltage from channel 1 (unit is mV)
voltage = idai.get_voltage 1
puts "Voltage (Channel 1): #{voltage/1000.0} V"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
