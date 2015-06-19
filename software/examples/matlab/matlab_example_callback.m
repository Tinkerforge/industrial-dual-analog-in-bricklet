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

    % Set Period (channel 1) for voltage callback to 1s (1000ms)
    % Note: The callback is only called every second if the
    %       voltage has changed since the last call!
    iain.setVoltageCallbackPeriod(1, 1000);

    % Register voltage callback to function cb_voltage
    set(iain, 'VoltageCallback', @(h, e) cb_voltage(e));

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_voltage(e)
    fprintf('Voltage [channel %g]: %g V\n', e.channel, e.voltage/1000);
end
