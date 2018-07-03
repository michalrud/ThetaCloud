#pragma once

#include "gmock/gmock.h"
#include <memory>

const uint8_t A0 = 0x76;

struct NiceArduinoMock
{
	MOCK_METHOD1(delay, void(uint8_t));
	MOCK_METHOD1(analogRead, int(uint8_t));
	MOCK_METHOD1(digitalRead, int(uint8_t));
	MOCK_METHOD2(pinMode, void(uint8_t, int));
};
using ArduinoMock = ::testing::StrictMock<NiceArduinoMock>;

extern std::unique_ptr<ArduinoMock> arduinoMock;

// randomly selected values, code should depend only on defines anyway
const int HIGH = 12;
const int LOW = 34;
const int OUTPUT = 56;
const int INPUT = 78;

void delay(uint8_t msecs);
int analogRead(uint8_t pin);
int digitalRead(uint8_t pin);
void pinMode(uint8_t pin, int mode);
