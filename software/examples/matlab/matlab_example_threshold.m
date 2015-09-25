function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDualAnalogIn;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    idai = BrickletIndustrialDualAnalogIn(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    idai.setDebouncePeriod(10000);

    % Register voltage reached callback to function cb_voltage_reached
    set(idai, 'VoltageReachedCallback', @(h, e) cb_voltage_reached(e));

    % Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
    idai.setVoltageCallbackThreshold(1, '>', 10*1000, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for voltage reached callback (parameter has unit mV)
function cb_voltage_reached(e)
    fprintf('Channel: %i\n', e.channel);
    fprintf('Voltage: %g V\n', e.voltage/1000.0);
    fprintf('\n');
end
