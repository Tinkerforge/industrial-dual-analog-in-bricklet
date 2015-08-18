Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    '  Callback for voltage greater than 5V
    Sub ReachedCB(ByVal sender As BrickletIndustrialDualAnalogIn,
                  ByVal channel As Byte, ByVal voltage As Integer)
        System.Console.WriteLine("Voltage (channel " + channel.ToString() + _
                                 ") is greater than 5V: " + _
                                 (voltage/1000.0).ToString() + "V")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idai As New BrickletIndustrialDualAnalogIn(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 1 seconds (1000ms)
        idai.SetDebouncePeriod(1000)

        ' Register threshold reached callback to function ReachedCB
        AddHandler idai.VoltageReached, AddressOf ReachedCB

        ' Configure threshold (channel 1) for "greater than 5V" (unit is mV)
        idai.SetVoltageCallbackThreshold(1, ">"C, 5*1000, 0)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
