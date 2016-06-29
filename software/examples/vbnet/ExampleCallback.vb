Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback subroutine for voltage callback (parameter has unit mV)
    Sub VoltageCB(ByVal sender As BrickletIndustrialDualAnalogIn, ByVal channel As Byte, _
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

        ' Register voltage callback to subroutine VoltageCB
        AddHandler idai.Voltage, AddressOf VoltageCB

        ' Set period for voltage (channel 1) callback to 1s (1000ms)
        ' Note: The voltage (channel 1) callback is only called every second
        '       if the voltage (channel 1) has changed since the last call!
        idai.SetVoltageCallbackPeriod(1, 1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
