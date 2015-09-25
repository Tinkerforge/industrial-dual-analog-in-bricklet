#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletIndustrialDualAnalogIn;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

# Callback subroutine for voltage callback (parameter has unit mV)
sub cb_voltage
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

# Register voltage callback to subroutine cb_voltage
$idai->register_callback($idai->CALLBACK_VOLTAGE, 'cb_voltage');

# Set period for voltage (channel 1) callback to 1s (1000ms)
# Note: The voltage (channel 1) callback is only called every second
#       if the voltage (channel 1) has changed since the last call!
$idai->set_voltage_callback_period(1, 1000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
