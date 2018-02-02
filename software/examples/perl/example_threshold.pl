#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDualAnalogIn;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Industrial Dual Analog In Bricklet

# Callback subroutine for voltage reached callback
sub cb_voltage_reached
{
    my ($channel, $voltage) = @_;

    print "Channel: $channel\n";
    print "Voltage: " . $voltage/1000.0 . " V\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $idai = Tinkerforge::BrickletIndustrialDualAnalogIn->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$idai->set_debounce_period(10000);

# Register voltage reached callback to subroutine cb_voltage_reached
$idai->register_callback($idai->CALLBACK_VOLTAGE_REACHED, 'cb_voltage_reached');

# Configure threshold for voltage (channel 1) "greater than 10 V"
$idai->set_voltage_callback_threshold(1, '>', 10*1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
