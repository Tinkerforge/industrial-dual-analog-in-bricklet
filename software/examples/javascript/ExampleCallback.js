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
        // Set Period (channel 1) for voltage callback to 1s (1000ms)
        // Note: The callback is only called every second if the
        // voltage has changed since the last call!
        iain.setVoltageCallbackPeriod(1, 1000);
    }
);
// Register voltage callback
iain.on(Tinkerforge.BrickletIndustrialDualAnalogIn.CALLBACK_VOLTAGE,
    // Callback function for voltage callback (parameter has unit V)
    function(channel, voltage) {
        console.log('Voltage (channel '+channel+'): '+voltage/1000+' V');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
