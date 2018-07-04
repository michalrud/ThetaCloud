#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudHumidity.h"
#include "DeviceHandlerToken.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "MockThetaCloud.hpp"
#include "generic_mocks.hpp"
#include <list>

using ::testing::_;
using ::testing::Return;

const uint8_t SHT21_ADDRESS = 0x40;
const uint8_t GET_TEMPERATURE_CMD = 0xF3;
const uint8_t GET_HUMIDITY_CMD = 0xF5;

struct ThetaCloudHumidityTests : public MockedThetaCloudTests
{
    ThetaCloudHumidity tested;
    MockCallback mockCallback;
    ::testing::Sequence s;
    std::list<ThetaCloud::DeviceReadHandler> handlers;

    ThetaCloudHumidityTests()
    {
        EXPECT_CALL((*mockThetaCloud),
                    addReadHandler(_)).WillRepeatedly(::testing::Invoke([&](const ThetaCloud::
                                                                            DeviceReadHandler
                                                                            &h){
            handlers.push_back(h);
            return DeviceHandlerTokenPtr();
        }));
        tested.init();

        EXPECT_EQ(handlers.size(), 2u);
    }
};

TEST_F(ThetaCloudHumidityTests, ReadingHumidity)
{
    expectWriteTo((*Wire.mock), s, SHT21_ADDRESS, {GET_HUMIDITY_CMD});
    expectReadFrom((*Wire.mock), s, SHT21_ADDRESS, {0xFF, 0x0B, 0x0C});
    EXPECT_CALL((*arduinoMock), delay(100)).Times(1);
    EXPECT_CALL(mockCallback,
                callback(::testing::_)).WillOnce(::testing::Invoke([](const SensorData &d){
        float value = std::stof(d.value);
        EXPECT_EQ(d.name, "humidity");
        EXPECT_GT(value, 118);
        EXPECT_LT(value, 119);
    }));

    handlers.front()(mockCallback.getCallback());
}

TEST_F(ThetaCloudHumidityTests, ReadingTemperature)
{
    expectWriteTo((*Wire.mock), s, SHT21_ADDRESS, {GET_TEMPERATURE_CMD});
    expectReadFrom((*Wire.mock), s, SHT21_ADDRESS, {0xFF, 0x0B, 0x0C});
    EXPECT_CALL((*arduinoMock), delay(100)).Times(1);
    EXPECT_CALL(mockCallback,
                callback(::testing::_)).WillOnce(::testing::Invoke([](const SensorData &d){
        float value = std::stof(d.value);
        EXPECT_EQ(d.name, "temperature");
        EXPECT_GT(value, 128);
        EXPECT_LT(value, 129);
    }));

    handlers.back()(mockCallback.getCallback());
}
