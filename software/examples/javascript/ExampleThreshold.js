var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Industrial Dual Analog In Bricklet

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var idai = new Tinkerforge.BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        idai.setDebouncePeriod(10000);

        // Configure threshold for voltage (channel 1) "greater than 10 V"
        idai.setVoltageCallbackThreshold(1, '>', 10*1000, 0);
    }
);

// Register voltage reached callback
idai.on(Tinkerforge.BrickletIndustrialDualAnalogIn.CALLBACK_VOLTAGE_REACHED,
    // Callback function for voltage reached callback
    function (channel, voltage) {
        console.log('Channel: ' + channel);
        console.log('Voltage: ' + voltage/1000.0 + ' V');
        console.log();
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
