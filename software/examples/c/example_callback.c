#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_industrial_dual_analog_in.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback function for voltage callback (parameter has unit mV)
void cb_voltage(uint8_t channel, int32_t voltage, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Voltage (channel %d): %fV\n", channel, voltage/1000.0);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	IndustrialDualAnalogIn iain;
	industrial_dual_analog_in_create(&iain, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Set Period for voltage callback for channel 1 to 1s (1000ms)
	// Note: The callback is only called every second if the
	//       voltage has changed since the last call!
	industrial_dual_analog_in_set_voltage_callback_period(&iain, 1, 1000);

	// Register voltage callback to function cb_voltage
	industrial_dual_analog_in_register_callback(&iain,
	                                            INDUSTRIAL_DUAL_ANALOG_IN_CALLBACK_VOLTAGE,
	                                            (void *)cb_voltage,
	                                            NULL);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
