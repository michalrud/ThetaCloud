#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudLight.h"
#include "Wire.h"
#include "testing_utils.hpp"
#include "generic_mocks.hpp"

using ::testing::_;
using ::testing::Return;

const uint8_t ISL29023_ADDRESS = 0x44;
const uint8_t CONTINOUS_AMBIENT_LIGHT_MEAS	= 0b10100000;
const uint8_t FULL_RANGE_64k_16BIT			= 0b00000011;
const uint8_t COMMAND1_REGISTER	= 0x00;
const uint8_t COMMAND2_REGISTER	= 0x01;
const uint8_t DATALSB_REGISTER	= 0x02;
const uint8_t DATAMSB_REGISTER	= 0x03;


struct ThetaCloudLightTests : public MockedThetaCloudTests
{
	ThetaCloudLight tested;
	MockCallback mockCallback;
	::testing::Sequence s;
};

TEST_F(ThetaCloudLightTests, ReadingLightSensor)
{
	ThetaCloud::DeviceReadHandler readHandler;
	EXPECT_CALL((*Wire.mock), beginTransmission(ISL29023_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(COMMAND1_REGISTER)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(CONTINOUS_AMBIENT_LIGHT_MEAS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), beginTransmission(ISL29023_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(COMMAND2_REGISTER)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(FULL_RANGE_64k_16BIT)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).WillOnce(::testing::Invoke([&](const ThetaCloud::DeviceReadHandler& h){
		readHandler = h;
		return DeviceHandlerTokenPtr();
	}));
	tested.init();

	EXPECT_CALL((*Wire.mock), beginTransmission(ISL29023_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(DATAMSB_REGISTER)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), requestFrom(ISL29023_ADDRESS, 1)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(1));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0A));
	EXPECT_CALL((*Wire.mock), beginTransmission(ISL29023_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(DATALSB_REGISTER)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), requestFrom(ISL29023_ADDRESS, 1)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(1));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0C));

	EXPECT_CALL(mockCallback, callback(_)).WillOnce(::testing::Invoke([](const SensorData& d){
		float value = std::stof(d.value);
		EXPECT_EQ(d.name, "light");
		EXPECT_GT(value, 2511);
		EXPECT_LT(value, 2512);
	}));
	readHandler(mockCallback.getCallback());
}
