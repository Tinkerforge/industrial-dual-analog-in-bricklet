function octave_example_simple()
    more off;
    
    HOST = "localhost";
    PORT = 4223;
    UID = "ftn"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    iain = java_new("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current voltage for channel 1 (unit is mV)
    voltage = iain.getVoltage(1);
    fprintf("Voltage: %g V\n", voltage/1000);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end
