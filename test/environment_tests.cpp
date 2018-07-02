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
	tested.init();

	EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(300));
	tested.init();
}

TEST_F(ThetaCloudEnvironmentTests, ReadingCo2AndVoc)
{
	ThetaCloud::DeviceReadHandler handler;
	std::map<std::string, float> reportedValues;

	EXPECT_CALL((*arduinoMock), analogRead(A0)).WillOnce(Return(200));
	EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).WillRepeatedly(::testing::Invoke([&](const ThetaCloud::DeviceReadHandler& h){
		handler = h;
		return DeviceHandlerTokenPtr();
	}));
	tested.init();

	EXPECT_CALL((*Wire.mock), beginTransmission(MICS_VZ_89_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(GET_STATUS_COMMAND)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), requestFrom(MICS_VZ_89_ADDRESS, 6)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(6));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0xFF));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0B));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0xFC));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0xFD));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0E));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0F));

	EXPECT_CALL(mockCallback, callback(_)).WillRepeatedly(::testing::Invoke([&](const SensorData& d){
		float value = std::stof(d.value);
		reportedValues[d.name] = value;
	}));

	handler(mockCallback.getCallback());

	EXPECT_EQ(reportedValues.at("co2"), 1852);
	EXPECT_EQ(reportedValues.at("voc"), 956);
}
