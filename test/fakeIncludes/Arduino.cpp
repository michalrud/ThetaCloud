#include "Arduino.h"

void delay(uint8_t msecs)
{
	arduinoMock->delay(msecs);
}