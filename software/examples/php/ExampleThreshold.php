<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDualAnalogIn.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDualAnalogIn;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback for voltage greater than 5V
function cb_reached($channel, $voltage)
{
    echo "Voltage (channel " . $channel . ") is greater than 5V: " . $voltage / 1000.0 . "\n";
}

$ipcon = new IPConnection(); // Create IP connection
$iain = new BrickletIndustrialDualAnalogIn(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$iain->setDebouncePeriod(10000);

// Register threshold reached callback to function cb_reached
$iain->registerCallback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE_REACHED, 'cb_reached');

// Configure threshold (channel 1) for "greater than 5V" (unit is mV)
$iain->setVoltageCallbackThreshold(1, '>', 5*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
