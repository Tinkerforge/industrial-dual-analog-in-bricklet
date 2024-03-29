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

	// Get current voltage from channel 1.
	voltage, _ := idai.GetVoltage(1)
	fmt.Printf("Voltage (Channel 1): %f V\n", float64(voltage)/1000.0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
