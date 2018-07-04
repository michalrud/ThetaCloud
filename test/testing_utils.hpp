#pragma once

#include <vector>
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

void expectWriteTo(WireMock &wireMock, ::testing::Sequence &s, uint8_t address,
                   std::vector<uint8_t> data)
{
    EXPECT_CALL(wireMock, beginTransmission(address)).InSequence(s);
    for (const auto &byte : data)
        EXPECT_CALL(wireMock, write(byte)).InSequence(s);
    EXPECT_CALL(wireMock, endTransmission()).InSequence(s);
}

void expectReadFrom(WireMock &wireMock, ::testing::Sequence &s, uint8_t address,
                    std::vector<uint8_t> data)
{
    EXPECT_CALL((*Wire.mock), requestFrom(address, data.size())).InSequence(s);
    EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(::testing::Return(data.size()));
    for (const auto &byte : data)
        EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(::testing::Return(byte));
}
