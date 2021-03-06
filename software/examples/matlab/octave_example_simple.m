function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Industrial Dual Analog In Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    idai = javaObject("com.tinkerforge.BrickletIndustrialDualAnalogIn", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current voltage from channel 1
    voltage = idai.getVoltage(1);
    fprintf("Voltage (Channel 1): %g V\n", voltage/1000.0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
