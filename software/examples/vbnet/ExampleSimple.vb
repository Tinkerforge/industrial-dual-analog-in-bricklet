Imports Tinkerforge

Module ExampleSimple
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idai As New BrickletIndustrialDualAnalogIn(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get current voltage from channel 1 (unit is mV)
        Dim voltage As Integer = idai.GetVoltage(1)
        System.Console.WriteLine("Voltage (Channel 1): " + (voltage/1000.0).ToString() + " V")

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
