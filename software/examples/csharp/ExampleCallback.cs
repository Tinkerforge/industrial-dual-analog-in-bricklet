using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for voltage callback (parameter has unit mV)
	static void VoltageCB(BrickletIndustrialDualAnalogIn sender, byte channel, int voltage)
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

		// Register voltage callback to function VoltageCB
		idai.Voltage += VoltageCB;

		// Set period for voltage (channel 1) callback to 1s (1000ms)
		// Note: The voltage (channel 1) callback is only called every second
		//       if the voltage (channel 1) has changed since the last call!
		idai.SetVoltageCallbackPeriod(1, 1000);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
