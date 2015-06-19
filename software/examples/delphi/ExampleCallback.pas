program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletIndustrialDualAnalogIn;

type
  TExample = class
  private
    ipcon: TIPConnection;
    iain: TBrickletIndustrialDualAnalogIn;
  public
    procedure VoltageCB(sender: TBrickletIndustrialDualAnalogIn;
                        const channel: byte; const voltage: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback function for voltage callback (parameter has unit mV) }
procedure TExample.VoltageCB(sender: TBrickletIndustrialDualAnalogIn;
                             const channel: byte; const voltage: longint);
begin
  WriteLn(Format('Voltage (channel %d): %f V', [channel, voltage/1000.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  iain := TBrickletIndustrialDualAnalogIn.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Set Period (channel 1) for voltage callback to 1s (1000ms)
    Note: The voltage callback is only called every second if the
          voltage has changed since the last call! }
  iain.SetVoltageCallbackPeriod(1, 1000);

  { Register voltage callback to procedure VoltageCB }
  iain.OnVoltage := {$ifdef FPC}@{$endif}VoltageCB;

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
