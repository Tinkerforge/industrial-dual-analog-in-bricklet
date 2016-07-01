using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Industrial Dual Analog In Bricklet

	// Callback function for voltage reached callback (parameter has unit mV)
	static void VoltageReachedCB(BrickletIndustrialDualAnalogIn sender, byte channel,
	                             int voltage)
	{
		Console.WriteLine("Channel: " + channel);
		Console.WriteLine("Voltage: " + voltage/1000.0 + " V");
		Console.WriteLine("");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn idai =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		idai.SetDebouncePeriod(10000);

		// Register voltage reached callback to function VoltageReachedCB
		idai.VoltageReached += VoltageReachedCB;

		// Configure threshold for voltage (channel 1) "greater than 10 V" (unit is mV)
		idai.SetVoltageCallbackThreshold(1, '>', 10*1000, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
