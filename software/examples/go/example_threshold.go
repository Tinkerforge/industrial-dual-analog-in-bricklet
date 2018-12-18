package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/industrial_dual_analog_in_bricklet"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Industrial Dual Analog In Bricklet.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	idai, _ := industrial_dual_analog_in_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get threshold receivers with a debounce time of 10 seconds (10000ms).
	idai.SetDebouncePeriod(10000)

	idai.RegisterVoltageReachedCallback(func(channel uint8, voltage int32) {
		fmt.Printf("Channel: %d\n", channel)
		fmt.Printf("Voltage: %f V\n", float64(voltage)/1000.0)
		fmt.Println()
	})

	// Configure threshold for voltage (channel 1) "greater than 10 V".
	idai.SetVoltageCallbackThreshold(1, '>', 10*1000, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
