using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback for voltage greater than 5V
	static void ReachedCB(BrickletIndustrialDualAnalogIn sender, byte channel, int voltage)
	{
		System.Console.WriteLine("Voltage (channel " + channel + ") is greater than 5V: " +
		                         voltage/1000.0 + "V");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn iain =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 1 seconds (1000ms)
		iain.SetDebouncePeriod(1000);

		// Register threshold reached callback to function ReachedCB
		iain.VoltageReached += ReachedCB;

		// Configure threshold (channel 1) for "greater than 5V" (unit is mV)
		iain.SetVoltageCallbackThreshold(1, '>', 5*1000, 0);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
