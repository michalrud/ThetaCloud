#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudEnvironment.h"
#include "DeviceHandlerToken.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "MockThetaCloud.hpp"
#include "generic_mocks.hpp"
#include <map>

using ::testing::_;
using ::testing::Return;

const uint8_t MICS_VZ_89_ADDRESS = 0x70;
const uint8_t GET_STATUS_COMMAND = 0b00001001;

struct ThetaCloudEnvironmentTests : public MockedThetaCloudTests
{
    ThetaCloudEnvironment tested;
    MockCallback mockCallback;
    ::testing::Sequence s;
};

TEST_F(ThetaCloudEnvironmentTests, NotConnectedBoardShouldNotDoAnythingInInit)
{
    EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).Times(0);

    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(0));
    tested.init(thetaCloud);

    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(300));
    tested.init(thetaCloud);
}

TEST_F(ThetaCloudEnvironmentTests, ReadingCo2AndVoc)
{
    ThetaCloud::DeviceReadHandler handler;
    std::map<std::string, float> reportedValues;

    EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(200));
    EXPECT_CALL((*mockThetaCloud),
                addReadHandler(_)).WillRepeatedly(::testing::Invoke([&](const ThetaCloud::
                                                                        DeviceReadHandler &h){
        handler = h;
        return DeviceHandlerTokenPtr();
    }));
    tested.init(thetaCloud);

    expectWriteTo((*Wire.mock), s, MICS_VZ_89_ADDRESS, {GET_STATUS_COMMAND});
    expectReadFrom((*Wire.mock), s, MICS_VZ_89_ADDRESS, {0xFF, 0x0B, 0xFC, 0xFD, 0x0E, 0x0F});

    EXPECT_CALL(mockCallback,
                callback(_)).WillRepeatedly(::testing::Invoke([&](const SensorData &d){
        float value = std::stof(d.value);
        reportedValues[d.name] = value;
    }));

    handler(mockCallback.getCallback());

    EXPECT_EQ(reportedValues.at("co2"), 1852);
    EXPECT_EQ(reportedValues.at("voc"), 956);
}
