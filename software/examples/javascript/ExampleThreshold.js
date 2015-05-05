var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change to your UID

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var iain = new Tinkerforge.BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        iain.setDebouncePeriod(10000);
        // Configure threshold (channel 1) for "greater than 5V" (unit is mV)
        iain.setVoltageCallbackThreshold(1, '>', 5*1000, 0);
    }
);

// Register threshold reached callback
iain.on(Tinkerforge.BrickletIndustrialDualAnalogIn.CALLBACK_VOLTAGE_REACHED,
    // Callback for voltage greater than 5V
    function(channel, voltage) {
        console.log('Voltage (channel '+channel+') is greater than 5V: '+voltage/1000+' V');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
