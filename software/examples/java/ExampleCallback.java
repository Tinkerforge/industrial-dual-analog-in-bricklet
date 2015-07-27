import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletIndustrialDualAnalogIn;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;
	private static final String UID = "XYZ"; // Change to your UID

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletIndustrialDualAnalogIn iain =
		  new BrickletIndustrialDualAnalogIn(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set Period (channel 1) for voltage callback to 1s (1000ms)
		// Note: The voltage callback is only called every second if the 
		//       voltage has changed since the last call!
		iain.setVoltageCallbackPeriod((short)1, 1000);

		// Add and implement voltage listener (called if voltage changes)
		iain.addVoltageListener(new BrickletIndustrialDualAnalogIn.VoltageListener() {
			public void voltage(short channel, int voltage) {
				System.out.println("Voltage (channel " + channel + "): " +
				                   voltage/1000.0 + " V");
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
