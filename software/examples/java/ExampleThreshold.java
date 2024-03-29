import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDualAnalogIn;

public class ExampleThreshold {
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

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		idai.setDebouncePeriod(10000);

		// Add voltage reached listener
		idai.addVoltageReachedListener(new BrickletIndustrialDualAnalogIn.VoltageReachedListener() {
			public void voltageReached(short channel, int voltage) {
				System.out.println("Channel: " + channel);
				System.out.println("Voltage: " + voltage/1000.0 + " V");
				System.out.println("");
			}
		});

		// Configure threshold for voltage (channel 1) "greater than 10 V"
		idai.setVoltageCallbackThreshold((short)1, '>', 10*1000, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
