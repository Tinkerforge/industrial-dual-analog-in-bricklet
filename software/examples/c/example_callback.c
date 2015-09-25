#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_industrial_dual_analog_in.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback function for voltage callback (parameter has unit mV)
void cb_voltage(uint8_t channel, int32_t voltage, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Channel: %d\n", channel);
	printf("Voltage: %f V\n", voltage/1000.0);
	printf("\n");
}

int main(void) {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	IndustrialDualAnalogIn idai;
	industrial_dual_analog_in_create(&idai, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		return 1;
	}
	// Don't use device before ipcon is connected

	// Register voltage callback to function cb_voltage
	industrial_dual_analog_in_register_callback(&idai,
	                                            INDUSTRIAL_DUAL_ANALOG_IN_CALLBACK_VOLTAGE,
	                                            (void *)cb_voltage,
	                                            NULL);

	// Set period for voltage (channel 1) callback to 1s (1000ms)
	// Note: The voltage (channel 1) callback is only called every second
	//       if the voltage (channel 1) has changed since the last call!
	industrial_dual_analog_in_set_voltage_callback_period(&idai, 1, 1000);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
	return 0;
}
