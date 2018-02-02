import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDualAnalogIn;

public class ExampleCallback {
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

		// Add voltage listener
		idai.addVoltageListener(new BrickletIndustrialDualAnalogIn.VoltageListener() {
			public void voltage(short channel, int voltage) {
				System.out.println("Channel: " + channel);
				System.out.println("Voltage: " + voltage/1000.0 + " V");
				System.out.println("");
			}
		});

		// Set period for voltage (channel 1) callback to 1s (1000ms)
		// Note: The voltage (channel 1) callback is only called every second
		//       if the voltage (channel 1) has changed since the last call!
		idai.setVoltageCallbackPeriod((short)1, 1000);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
