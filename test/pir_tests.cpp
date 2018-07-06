#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudPIR.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "generic_mocks.hpp"
#include <list>

using ::testing::_;
using ::testing::Return;

const uint8_t PIR1_PIN = 12;
const uint8_t PIR2_PIN = 13;
const std::string PIR1_NAME = "PIR1";
const std::string PIR2_NAME = "PIR2";

struct ThetaCloudPIRNotConnectedTests : public MockedThetaCloudTests
{
    ThetaCloudPIR tested;
};

struct ThetaCloudPIRTests : public MockedThetaCloudTests
{
    ThetaCloudPIR tested;
    MockCallback mockCallback;
    std::list<ThetaCloud::DeviceReadHandler> readHandlers;

    ThetaCloudPIRTests()
    {
        EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(100));
        EXPECT_CALL((*arduinoMock), pinMode(PIR1_PIN, INPUT));
        EXPECT_CALL((*arduinoMock), pinMode(PIR2_PIN, INPUT));
        EXPECT_CALL((*mockThetaCloud), addReadHandler(_))
        .Times(2)
        .WillRepeatedly(::testing::Invoke([&](const ThetaCloud::DeviceReadHandler &h){
            readHandlers.push_back(h);
            return DeviceHandlerTokenPtr();
        }));

        tested.init(thetaCloud);
    }
};

TEST_F(ThetaCloudPIRNotConnectedTests, A0_too_low_nothing_happens)
{
    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(0));
    tested.init(thetaCloud);
}

TEST_F(ThetaCloudPIRNotConnectedTests, A0_too_high_nothing_happens)
{
    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(200));
    tested.init(thetaCloud);
}

TEST_F(ThetaCloudPIRTests, ReadingPIRValue_still_low)
{
    EXPECT_CALL(mockCallback, callback(_)).Times(0);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_CALL((*arduinoMock), digitalRead(PIR1_PIN)).WillOnce(Return(LOW));
        EXPECT_CALL((*arduinoMock), digitalRead(PIR2_PIN)).WillOnce(Return(LOW));
        for (const auto &h : readHandlers) h(mockCallback.getCallback());
    }
}

TEST_F(ThetaCloudPIRTests, ReadingPIRValue_still_high)
{
    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR1_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR2_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_CALL((*arduinoMock), digitalRead(PIR1_PIN)).WillOnce(Return(HIGH));
        EXPECT_CALL((*arduinoMock), digitalRead(PIR2_PIN)).WillOnce(Return(HIGH));
        for (const auto &h : readHandlers) h(mockCallback.getCallback());
    }
}
TEST_F(ThetaCloudPIRTests, ReadingPIRValue_high_then_low_then_high_again)
{
    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR1_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR2_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_CALL((*arduinoMock), digitalRead(PIR1_PIN)).WillOnce(Return(HIGH));
        EXPECT_CALL((*arduinoMock), digitalRead(PIR2_PIN)).WillOnce(Return(HIGH));
        for (const auto &h : readHandlers) h(mockCallback.getCallback());
    }

    EXPECT_CALL(mockCallback, callback(SensorData{PIR1_NAME, ThetaCloudPIR::MOTION_LOST})).Times(1);
    EXPECT_CALL(mockCallback, callback(SensorData{PIR2_NAME, ThetaCloudPIR::MOTION_LOST})).Times(1);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_CALL((*arduinoMock), digitalRead(PIR1_PIN)).WillOnce(Return(LOW));
        EXPECT_CALL((*arduinoMock), digitalRead(PIR2_PIN)).WillOnce(Return(LOW));
        for (const auto &h : readHandlers) h(mockCallback.getCallback());
    }

    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR1_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    EXPECT_CALL(mockCallback,
                callback(SensorData{PIR2_NAME, ThetaCloudPIR::MOTION_DETECTED})).Times(1);
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_CALL((*arduinoMock), digitalRead(PIR1_PIN)).WillOnce(Return(HIGH));
        EXPECT_CALL((*arduinoMock), digitalRead(PIR2_PIN)).WillOnce(Return(HIGH));
        for (const auto &h : readHandlers) h(mockCallback.getCallback());
    }
}
