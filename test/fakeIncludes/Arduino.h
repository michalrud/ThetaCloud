#pragma once

#include "gmock/gmock.h"
#include <memory>

struct ArduinoMock
{
	MOCK_METHOD1(delay, void(uint8_t));
};

extern std::unique_ptr<ArduinoMock> arduinoMock;

void delay(uint8_t msecs);
