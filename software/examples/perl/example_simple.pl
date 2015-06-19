#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDualAnalogIn;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $iain = Tinkerforge::BrickletIndustrialDualAnalogIn->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current voltage for channel 1 (unit is mV)
my $voltage = $iain->get_voltage(1)/1000.0;
print "Voltage: $voltage V\n";

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

