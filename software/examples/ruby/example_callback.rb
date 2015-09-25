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

# Register voltage callback (parameter has unit mV)
idai.register_callback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE) do |channel, voltage|
  puts "Channel: #{channel}"
  puts "Voltage: #{voltage/1000.0} V"
  puts ''
end

# Set period for voltage (channel 1) callback to 1s (1000ms)
# Note: The voltage (channel 1) callback is only called every second
#       if the voltage (channel 1) has changed since the last call!
idai.set_voltage_callback_period 1, 1000

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
