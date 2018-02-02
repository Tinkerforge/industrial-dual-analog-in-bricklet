<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDualAnalogIn.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDualAnalogIn;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Industrial Dual Analog In Bricklet

// Callback function for voltage callback
function cb_voltage($channel, $voltage)
{
    echo "Channel: $channel\n";
    echo "Voltage: " . $voltage/1000.0 . " V\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$idai = new BrickletIndustrialDualAnalogIn(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register voltage callback to function cb_voltage
$idai->registerCallback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE, 'cb_voltage');

// Set period for voltage (channel 1) callback to 1s (1000ms)
// Note: The voltage (channel 1) callback is only called every second
//       if the voltage (channel 1) has changed since the last call!
$idai->setVoltageCallbackPeriod(1, 1000);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
