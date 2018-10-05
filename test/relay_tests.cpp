#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudRelay.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "generic_mocks.hpp"
#include <list>

using ::testing::_;
using ::testing::Return;

const uint8_t RELAY1_PIN = 12;
const uint8_t RELAY2_PIN = 13;
const std::string RELAY1_NAME = "Relay1";
const std::string RELAY2_NAME = "Relay2";

struct ThetaCloudRelayNotInitializedTests : public MockedThetaCloudTests
{
    ThetaCloudRelay tested;
};

struct ThetaCloudRelayTests : public MockedThetaCloudTests
{
    ThetaCloudRelay tested;
    MockCallback mockCallback;
    ThetaCloud::DeviceWriteHandler relay1WriteHandler;
    ThetaCloud::DeviceWriteHandler relay2WriteHandler;

    ThetaCloudRelayTests()
    {
        EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(300));
        EXPECT_CALL((*arduinoMock), pinMode(RELAY1_PIN, OUTPUT));
        EXPECT_CALL((*arduinoMock), pinMode(RELAY2_PIN, OUTPUT));
        EXPECT_CALL((*mockThetaCloud), addWriteHandler("Relay1", _))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([&](const std::string &,
                                              const ThetaCloud::DeviceWriteHandler &h){
            relay1WriteHandler = h;
            return DeviceHandlerTokenPtr();
        }));
        EXPECT_CALL((*mockThetaCloud), addWriteHandler("Relay2", _))
        .Times(1)
        .WillRepeatedly(::testing::Invoke([&](const std::string &,
                                              const ThetaCloud::DeviceWriteHandler &h){
            relay2WriteHandler = h;
            return DeviceHandlerTokenPtr();
        }));

        tested.init(thetaCloud);
    }
};

TEST_F(ThetaCloudRelayNotInitializedTests, A0_too_low_nothing_happens)
{
    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(200));
    tested.init(thetaCloud);
}
TEST_F(ThetaCloudRelayNotInitializedTests, A0_too_high_nothing_happens)
{
    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(400));
    tested.init(thetaCloud);
}

TEST_F(ThetaCloudRelayTests, Enable_Relay1)
{
    EXPECT_CALL((*arduinoMock), digitalWrite(RELAY1_PIN, HIGH));
    relay1WriteHandler(SensorData{"Relay1", "ON"}, mockCallback.getCallback());
}

TEST_F(ThetaCloudRelayTests, Enable_Relay2)
{
    EXPECT_CALL((*arduinoMock), digitalWrite(RELAY2_PIN, HIGH));
    relay2WriteHandler(SensorData{"Relay2", "ON"}, mockCallback.getCallback());
}

TEST_F(ThetaCloudRelayTests, Disable_Relay1)
{
    EXPECT_CALL((*arduinoMock), digitalWrite(RELAY1_PIN, LOW));
    relay1WriteHandler(SensorData{"Relay1", "OFF"}, mockCallback.getCallback());
}

TEST_F(ThetaCloudRelayTests, Disable_Relay2)
{
    EXPECT_CALL((*arduinoMock), digitalWrite(RELAY2_PIN, LOW));
    relay2WriteHandler(SensorData{"Relay2", "OFF"}, mockCallback.getCallback());
}

TEST_F(ThetaCloudRelayTests, Relay1_unknown_command_doesnt_do_anything)
{
    relay1WriteHandler(SensorData{"Relay1", "unknown command"}, mockCallback.getCallback());
}

TEST_F(ThetaCloudRelayTests, Relay2_unknown_command_doesnt_do_anything)
{
    relay2WriteHandler(SensorData{"Relay2", "unknown command"}, mockCallback.getCallback());
}
