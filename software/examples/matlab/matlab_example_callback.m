function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletIndustrialDualAnalogIn;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    idai = BrickletIndustrialDualAnalogIn(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register voltage callback to function cb_voltage
    set(idai, 'VoltageCallback', @(h, e) cb_voltage(e));

    % Set period for voltage (channel 1) callback to 1s (1000ms)
    % Note: The voltage (channel 1) callback is only called every second
    %       if the voltage (channel 1) has changed since the last call!
    idai.setVoltageCallbackPeriod(1, 1000);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for voltage callback (parameter has unit mV)
function cb_voltage(e)
    fprintf('Channel: %i\n', e.channel);
    fprintf('Voltage: %g V\n', e.voltage/1000.0);
    fprintf('\n');
end
