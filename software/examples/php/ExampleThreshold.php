<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDualAnalogIn.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDualAnalogIn;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for voltage reached callback (parameter has unit mV)
function cb_voltageReached($channel, $voltage)
{
    echo "Channel: $channel\n";
    echo "Voltage: " . $voltage/1000.0 . " V\n";
    echo "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$idai = new BrickletIndustrialDualAnalogIn(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$idai->setDebouncePeriod(10000);

// Register voltage reached callback to function cb_voltageReached
$idai->registerCallback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE_REACHED, 'cb_voltageReached');

// Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
$idai->setVoltageCallbackThreshold(1, '>', 10*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
