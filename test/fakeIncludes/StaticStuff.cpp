#include "Wire.h"
#include "Arduino.h"

WireProxy Wire;
std::unique_ptr<ArduinoMock> arduinoMock;
