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
	ThetaCloud::DeviceReadHandler readHandler;
	::testing::Sequence s;

	ThetaCloudLightTests()
	{
		expectWriteTo((*Wire.mock), s, ISL29023_ADDRESS, {COMMAND1_REGISTER, CONTINOUS_AMBIENT_LIGHT_MEAS});
		expectWriteTo((*Wire.mock), s, ISL29023_ADDRESS, {COMMAND2_REGISTER, FULL_RANGE_64k_16BIT});
		EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).WillOnce(::testing::Invoke([&](const ThetaCloud::DeviceReadHandler& h){
			readHandler = h;
			return DeviceHandlerTokenPtr();
		}));
		tested.init();
	}
};

TEST_F(ThetaCloudLightTests, ReadingLightSensor)
{
	expectWriteTo((*Wire.mock), s, ISL29023_ADDRESS, {DATAMSB_REGISTER});
	expectReadFrom((*Wire.mock), s, ISL29023_ADDRESS, {0x0A});
	expectWriteTo((*Wire.mock), s, ISL29023_ADDRESS, {DATALSB_REGISTER});
	expectReadFrom((*Wire.mock), s, ISL29023_ADDRESS, {0x0C});

	EXPECT_CALL(mockCallback, callback(_)).WillOnce(::testing::Invoke([](const SensorData& d){
		float value = std::stof(d.value);
		EXPECT_EQ(d.name, "light");
		EXPECT_GT(value, 2511);
		EXPECT_LT(value, 2512);
	}));
	readHandler(mockCallback.getCallback());
}
