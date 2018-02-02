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
        // Get current voltage from channel 1
        idai.getVoltage(1,
            function (voltage) {
                console.log('Voltage (Channel 1): ' + voltage/1000.0 + ' V');
            },
            function (error) {
                console.log('Error: ' + error);
            }
        );
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
