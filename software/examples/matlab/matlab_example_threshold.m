function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDualAnalogIn;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'ftn'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    iain = BrickletIndustrialDualAnalogIn(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    iain.setDebouncePeriod(10000);

    % Register threshold reached callback to function cb_reached
    set(iain, 'VoltageReachedCallback', @(h, e) cb_reached(e));

    % Configure threshold (channel 1) for "greater than 5V" (unit is mV)
    iain.setVoltageCallbackThreshold(1, '>', 5*1000*1000, 0);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback for voltage greater than 5V
function cb_reached(e)
    fprintf('Voltage [channel %g]: %g mA\n', e.channel, e.voltage/1000);
end
