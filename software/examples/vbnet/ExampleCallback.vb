Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback function for voltage callback (parameter has unit mV)
    Sub VoltageCB(ByVal sender As BrickletIndustrialDualAnalogIn, _
                  ByVal channel As Byte, ByVal voltage As Integer)
        System.Console.WriteLine("Voltage (channel" + channel.ToString() + _
                                 "): " + (voltage/1000.0).ToString() + " V")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim idai As New BrickletIndustrialDualAnalogIn(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set Period (channel 1) for voltage callback to 1s (1000ms)
        ' Note: The voltage callback is only called every second if the 
        '       voltage has changed since the last call!
        idai.SetVoltageCallbackPeriod(1, 1000)

        ' Register voltage callback to function VoltageCB
        AddHandler idai.Voltage, AddressOf VoltageCB

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
