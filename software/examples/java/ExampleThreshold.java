import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDualAnalogIn;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn idai =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		idai.setDebouncePeriod(10000);

		// Configure threshold (channel 1) for "greater than 5V" (unit is mV)
		idai.setVoltageCallbackThreshold((short)1, '>', 5*1000, 0);

		// Add and implement voltage reached listener (called if voltage is greater than 5V)
		idai.addVoltageReachedListener(new BrickletIndustrialDualAnalogIn.VoltageReachedListener() {
			public void voltageReached(short channel, int voltage) {
				System.out.println("Voltage (Channel " + channel + ") is greater than 5V: " +
				                   voltage/1000.0);
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
