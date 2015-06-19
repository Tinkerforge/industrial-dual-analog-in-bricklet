#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_industrial_dual_analog_in.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

// Callback for voltage greater than 5V
void cb_reached(uint8_t channel, int32_t voltage, void *user_data) {
	(void)user_data; // avoid unused parameter warning

	printf("Voltage (channel %d) is greater than 5V: %f\n", channel, voltage/1000.0);
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

	// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
	industrial_dual_analog_in_set_debounce_period(&iain, 10000);

	// Register threshold reached callback to function cb_reached
	industrial_dual_analog_in_register_callback(&iain,
	                                            INDUSTRIAL_DUAL_ANALOG_IN_CALLBACK_VOLTAGE_REACHED,
	                                            (void *)cb_reached,
	                                            NULL);

	// Configure threshold (channel 1) for "greater than 5V" (unit is mV)
	industrial_dual_analog_in_set_voltage_callback_threshold(&iain, 1, '>', 5*1000, 0);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon);
}
