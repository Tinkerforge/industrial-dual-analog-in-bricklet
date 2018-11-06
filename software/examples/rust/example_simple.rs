use std::{error::Error, io};

use tinkerforge::{industrial_dual_analog_in_bricklet::*, ipconnection::IpConnection};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Dual Analog In Bricklet

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let industrial_dual_analog_in_bricklet = IndustrialDualAnalogInBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    // Get current voltage from channel 1
    let voltage = industrial_dual_analog_in_bricklet.get_voltage(1).recv()?;
    println!("Voltage (Channel 1): {}{}", voltage as f32 / 1000.0, " V");

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
