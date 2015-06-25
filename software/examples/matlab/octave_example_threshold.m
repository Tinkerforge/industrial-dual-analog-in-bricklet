function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    iain = java_new("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    iain.setDebouncePeriod(10000);

    % Register threshold reached callback to function cb_reached
    iain.addVoltageReachedCallback(@cb_reached);

    % Configure threshold (channel 1) for "greater than 5V" (unit is mV)
    iain.setVoltageCallbackThreshold(1, iain.THRESHOLD_OPTION_GREATER, 5*1000, 0);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_reached(e)
    fprintf("Voltage [channel %d]: %g V\n", short2int(e.channel), e.voltage/1000);
end

function int = short2int(short)
    if compare_versions(version(), "3.8", "<=")
        int = short.intValue();
    else
        int = short;
    end
end
