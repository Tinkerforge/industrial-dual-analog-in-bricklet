Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback subroutine for voltage reached callback (parameter has unit mV)
    Sub VoltageReachedCB(ByVal sender As BrickletIndustrialDualAnalogIn, ByVal channel As Byte, _
                         ByVal voltage As Integer)
        Console.WriteLine("Channel: " + channel.ToString())
        Console.WriteLine("Voltage: " + (voltage/1000.0).ToString() + " V")
        Console.WriteLine("")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idai As New BrickletIndustrialDualAnalogIn(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        idai.SetDebouncePeriod(10000)

        ' Register voltage reached callback to subroutine VoltageReachedCB
        AddHandler idai.VoltageReached, AddressOf VoltageReachedCB

        ' Configure threshold for voltage "greater than 10 V" (unit is mV)
        idai.SetVoltageCallbackThreshold(1, ">"C, 10*1000, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
