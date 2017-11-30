#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_industrial_dual_analog_in'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Industrial Dual Analog In Bricklet

ipcon = IPConnection.new # Create IP connection
idai = BrickletIndustrialDualAnalogIn.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
idai.set_debounce_period 10000

# Register voltage reached callback (parameter has unit mV)
idai.register_callback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE_REACHED) do |channel,
                                                                                     voltage|
  puts "Channel: #{channel}"
  puts "Voltage: #{voltage/1000.0} V"
  puts ''
end

# Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
idai.set_voltage_callback_threshold 1, '>', 10*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
