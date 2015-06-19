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

# Set Period (channel 1) for voltage callback to 1s (1000ms)
# Note: The callback is only called every second if the
#       voltage has changed since the last call!
iain.set_voltage_callback_period 1, 1000

# Register voltage callback (parameter has unit mV)
iain.register_callback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE) do |channel, voltage|
  puts "Voltage (channel #{channel}): #{voltage/1000.0} V"
end

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
