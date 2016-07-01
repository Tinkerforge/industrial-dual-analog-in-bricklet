function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDualAnalogIn;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Industrial Dual Analog In Bricklet

    ipcon = IPConnection(); % Create IP connection
    idai = handle(BrickletIndustrialDualAnalogIn(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current voltage from channel 1 (unit is mV)
    voltage = idai.getVoltage(1);
    fprintf('Voltage (Channel 1): %g V\n', voltage/1000.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
