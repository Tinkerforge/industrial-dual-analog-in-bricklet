function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual Analog In Bricklet

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    idai = java_new("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    idai.setDebouncePeriod(10000);

    % Register voltage reached callback to function cb_voltage_reached
    idai.addVoltageReachedCallback(@cb_voltage_reached);

    % Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
    idai.setVoltageCallbackThreshold(1, ">", 10*1000, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage reached callback (parameter has unit mV)
function cb_voltage_reached(e)
    fprintf("Channel: %d\n", java2int(e.channel));
    fprintf("Voltage: %g V\n", e.voltage/1000.0);
    fprintf("\n");
end

function int = java2int(value)
    if compare_versions(version(), "3.8", "<=")
        int = value.intValue();
    else
        int = value;
    end
end
