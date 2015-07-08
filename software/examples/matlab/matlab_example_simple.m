function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDualAnalogIn;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    iain = BrickletIndustrialDualAnalogIn(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current voltage for channel 1 (unit is mV)
    voltage = iain.getVoltage(1);
    fprintf('Voltage: %g V\n', voltage/1000);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end
