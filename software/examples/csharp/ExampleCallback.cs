using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for voltage callback (parameter has unit mV)
	static void VoltageCB(BrickletIndustrialDualAnalogIn sender, byte channel, int voltage)
	{
		System.Console.WriteLine("Voltage (channel " + channel + "): " +
		                         voltage/1000.0 + " V");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn iain =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set Period (channel 1) for voltage callback to 1s (1000ms)
		// Note: The voltage callback is only called every second if the 
		//       voltage has changed since the last call!
		iain.SetVoltageCallbackPeriod(1, 1000);

		// Register voltage callback to function VoltageCB
		iain.Voltage += VoltageCB;

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
