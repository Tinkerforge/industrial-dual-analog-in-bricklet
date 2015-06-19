function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "ftn"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    iain = java_new("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set Period (channel 1) for voltage callback to 1s (1000ms)
    % Note: The callback is only called every second if the
    %       voltage has changed since the last call!
    iain.setVoltageCallbackPeriod(1, 1000);

    % Register voltage callback to function cb_voltage
    iain.addVoltageCallback(@cb_voltage);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_voltage(e)
    fprintf("Voltage [channel %s]: %g V\n", e.channel.toString(), e.voltage/1000);
end
