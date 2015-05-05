<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDualAnalogIn.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDualAnalogIn;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

// Callback function for voltage callback (parameter has unit mV)
function cb_voltage($channel, $voltage)
{
    echo "Voltage (channel " . $channel . "): " . $voltage / 1000.0 . " V\n";
}

$ipcon = new IPConnection(); // Create IP connection
$iain = new BrickletIndustrialDualAnalogIn(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set Period (channel 1) for voltage callback to 1s (1000ms)
// Note: The callback is only called every second if the 
//       voltage has changed since the last call!
$iain->setVoltageCallbackPeriod(1, 1000);

// Register voltage callback to function cb_voltage
$iain->registerCallback(BrickletIndustrialDualAnalogIn::CALLBACK_VOLTAGE, 'cb_voltage');

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
