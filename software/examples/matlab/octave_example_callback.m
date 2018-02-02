function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual Analog In Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    idai = javaObject("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register voltage callback to function cb_voltage
    idai.addVoltageCallback(@cb_voltage);

    % Set period for voltage (channel 1) callback to 1s (1000ms)
    % Note: The voltage (channel 1) callback is only called every second
    %       if the voltage (channel 1) has changed since the last call!
    idai.setVoltageCallbackPeriod(1, 1000);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage callback
function cb_voltage(e)
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
