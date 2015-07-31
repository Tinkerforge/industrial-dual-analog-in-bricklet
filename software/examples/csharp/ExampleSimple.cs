using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn idai =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current voltage from channel 1 (unit is mV)
		int voltage = idai.GetVoltage(1);
		System.Console.WriteLine("Voltage (Channel 1): " + voltage/1000.0 + " V");

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
