#include "Arduino.h"

void delay(uint8_t msecs)
{
	arduinoMock->delay(msecs);
}

int analogRead(uint8_t pin)
{
	return arduinoMock->analogRead(pin);
}
