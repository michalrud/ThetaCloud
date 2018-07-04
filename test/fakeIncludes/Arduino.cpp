#include "Arduino.h"

void delay(uint8_t msecs)
{
    arduinoMock->delay(msecs);
}

int analogRead(uint8_t pin)
{
    return arduinoMock->analogRead(pin);
}

int digitalRead(uint8_t pin)
{
    return arduinoMock->digitalRead(pin);
}

void pinMode(uint8_t pin, int pinMode)
{
    arduinoMock->pinMode(pin, pinMode);
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    arduinoMock->digitalWrite(pin, value);
}
