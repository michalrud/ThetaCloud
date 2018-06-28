#pragma once

#include "gtest/gtest.h"
#include <SensorData.h>
#include <Wire.h>
#include <Arduino.h>

struct GenericTest : public ::testing::Test {
	GenericTest()
	{
		Wire.mock.reset(new WireMock());
		arduinoMock.reset(new ArduinoMock());
	}
	~GenericTest()
	{
		Wire.mock.reset();
		arduinoMock.reset();
	}
};

bool operator ==(const SensorData &a, const SensorData &b)
{
	return a.name == b.name && a.value == b.value;
}
