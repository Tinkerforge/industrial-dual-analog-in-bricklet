#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDualAnalogIn;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $iain = Tinkerforge::BrickletIndustrialDualAnalogIn->new(&UID, $ipcon); # Create device object

# Callback function for voltage callback (parameter has unit mV)
sub cb_voltage
{
    my ($channel, $voltage) = @_;

    print "Voltage (channel $channel): ".$voltage/1000.0." V\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set Period (channel 1) for voltage callback to 1s (1000ms)
# Note: The callback is only called every second if the 
#       voltage has changed since the last call!
$iain->set_voltage_callback_period(1, 1000);

# Register voltage callback to function cb_voltage
$iain->register_callback($iain->CALLBACK_VOLTAGE, 'cb_voltage');

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

