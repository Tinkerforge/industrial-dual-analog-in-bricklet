import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDualAnalogIn;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Industrial Dual Analog In Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn idai =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current voltage from channel 1 (unit is mV)
		int voltage = idai.getVoltage((short)1); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Voltage (Channel 1): " + voltage/1000.0 + " V");

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
