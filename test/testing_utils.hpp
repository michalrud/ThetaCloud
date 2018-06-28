#pragma once

#include "gtest/gtest.h"
#include <SensorData.h>
#include <Wire.h>
#include <Arduino.h>
#include "ThetaCloud.h"
#include "MockThetaCloud.hpp"

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

struct MockedThetaCloudTests : public GenericTest {
	MockedThetaCloudTests()
	{
		mockThetaCloud.reset(new MockThetaCloud());
	}
	~MockedThetaCloudTests()
	{
		mockThetaCloud.reset();
	}
};

bool operator ==(const SensorData &a, const SensorData &b)
{
	return a.name == b.name && a.value == b.value;
}
