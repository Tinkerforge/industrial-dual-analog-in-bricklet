#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_dual_analog_in'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
iain = BrickletIndustrialDualAnalogIn.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
iain.set_debounce_period 10000

# Register threshold reached callback (unit is mV)
iain.register_callback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE_REACHED) do |channel, voltage|
  puts "Voltage (channel #{channel}) is greater than 5V: #{voltage/1000.0}"
end

# Configure threshold (channel 1) for "greater than 5V" (unit is mV)
iain.set_voltage_callback_threshold 1, '>', 5*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
