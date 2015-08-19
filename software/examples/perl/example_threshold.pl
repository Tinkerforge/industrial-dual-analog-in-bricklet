#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDualAnalogIn;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $idai = Tinkerforge::BrickletIndustrialDualAnalogIn->new(&UID, $ipcon); # Create device object

# Callback for voltage greater than 5V
sub cb_reached
{
    my ($channel, $voltage) = @_;

    print "Voltage (channel $channel) is greather than 5V: ".$voltage/1000.0." V\n";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$idai->set_debounce_period(10000);

# Register threshold reached callback to function cb_reached
$idai->register_callback($idai->CALLBACK_VOLTAGE_REACHED, 'cb_reached');

# Configure threshold (channel 1) for "greater than 5V" (unit is mV)
$idai->set_voltage_callback_threshold(1, '>', 5*1000, 0);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

