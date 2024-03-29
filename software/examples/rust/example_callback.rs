use std::{error::Error, io, thread};
use tinkerforge::{industrial_dual_analog_in_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Industrial Dual Analog In Bricklet.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let idai = IndustrialDualAnalogInBricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    let voltage_receiver = idai.get_voltage_callback_receiver();

    // Spawn thread to handle received callback messages.
    // This thread ends when the `idai` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for voltage in voltage_receiver {
            println!("Channel: {}", voltage.channel);
            println!("Voltage: {} V", voltage.voltage as f32 / 1000.0);
            println!();
        }
    });

    // Set period for voltage (channel 1) receiver to 1s (1000ms).
    // Note: The voltage (channel 1) callback is only called every second
    //       if the voltage (channel 1) has changed since the last call!
    idai.set_voltage_callback_period(1, 1000);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
