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

	idai.RegisterVoltageCallback(func(channel uint8, voltage int32) {
		fmt.Printf("Channel: %d\n", channel)
		fmt.Printf("Voltage: %f V\n", float64(voltage)/1000.0)
		fmt.Println()
	})

	// Set period for voltage (channel 1) receiver to 1s (1000ms).
	// Note: The voltage (channel 1) callback is only called every second
	//       if the voltage (channel 1) has changed since the last call!
	idai.SetVoltageCallbackPeriod(1, 1000)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
