<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletIndustrialDualAnalogIn.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletIndustrialDualAnalogIn;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$idai = new BrickletIndustrialDualAnalogIn(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current voltage from channel 1 (unit is mV)
$voltage = $idai->getVoltage(1);
echo "Voltage (Channel 1): " . $voltage/1000.0 . " V\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
