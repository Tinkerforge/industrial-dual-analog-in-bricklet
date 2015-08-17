program ExampleThreshold;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletIndustrialDualAnalogIn;

type
  TExample = class
  private
    ipcon: TIPConnection;
    idai: TBrickletIndustrialDualAnalogIn;
  public
    procedure ReachedCB(sender: TBrickletIndustrialDualAnalogIn;
                        const channel: byte; const voltage: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback for voltage greater than 5V }
procedure TExample.ReachedCB(sender: TBrickletIndustrialDualAnalogIn;
                             const channel: byte; const voltage: longint);
begin
  WriteLn(Format('Voltage (channel %d) is greater than 5V: %f V',
                 [channel, voltage/1000.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  idai := TBrickletIndustrialDualAnalogIn.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get threshold callbacks with a debounce time of 10 seconds (10000ms) }
  idai.SetDebouncePeriod(10000);

  { Register threshold reached callback to procedure ReachedCB }
  idai.OnVoltageReached := {$ifdef FPC}@{$endif}ReachedCB;

  { Configure threshold (channel 1) for "greater than 5V" (unit is mV) }
  idai.SetVoltageCallbackThreshold(1, '>', 5*1000, 0);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
