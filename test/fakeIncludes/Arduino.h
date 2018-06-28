#pragma once

#include "gmock/gmock.h"
#include <memory>

const uint8_t A0 = 0x76;

struct ArduinoMock
{
	MOCK_METHOD1(delay, void(uint8_t));
	MOCK_METHOD1(analogRead, int(uint8_t));
};

extern std::unique_ptr<ArduinoMock> arduinoMock;

void delay(uint8_t msecs);
int analogRead(uint8_t pin);
