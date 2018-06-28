#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "ThetaCloud.h"
#include "ThetaCloudHumidity.h"
#include "SensorHandlerToken.h"
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
};

TEST_F(ThetaCloudHumidityTests, ReadingHumidity)
{
	std::list<ThetaCloud::SensorReadHandler> handlers;
	EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).WillRepeatedly(::testing::Invoke([&](const ThetaCloud::SensorReadHandler& h){
		handlers.push_back(h);
		return SensorHandlerTokenPtr();
	}));
	tested.init();

	EXPECT_EQ(handlers.size(), 2);

	EXPECT_CALL((*Wire.mock), beginTransmission(SHT21_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(GET_HUMIDITY_CMD)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), requestFrom(SHT21_ADDRESS, 3)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(3));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0xFF));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0B));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0C));
	EXPECT_CALL((*arduinoMock), delay(100)).Times(1);
	EXPECT_CALL(mockCallback, callback(::testing::_)).WillOnce(::testing::Invoke([](const SensorData& d){
		float value = std::stof(d.value);
		EXPECT_EQ(d.name, "humidity");
		EXPECT_GT(value, 118);
		EXPECT_LT(value, 119);
	}));

	handlers.front()(mockCallback.getCallback());
}

TEST_F(ThetaCloudHumidityTests, ReadingTemperature)
{
	std::list<ThetaCloud::SensorReadHandler> handlers;
	EXPECT_CALL((*mockThetaCloud), addReadHandler(_)).WillRepeatedly(::testing::Invoke([&](const ThetaCloud::SensorReadHandler& h){
		handlers.push_back(h);
		return SensorHandlerTokenPtr();
	}));
	tested.init();

	EXPECT_EQ(handlers.size(), 2);

	EXPECT_CALL((*Wire.mock), beginTransmission(SHT21_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(GET_TEMPERATURE_CMD)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	EXPECT_CALL((*Wire.mock), requestFrom(SHT21_ADDRESS, 3)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(3));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0xFF));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0B));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0C));
	EXPECT_CALL((*arduinoMock), delay(100)).Times(1);
	EXPECT_CALL(mockCallback, callback(::testing::_)).WillOnce(::testing::Invoke([](const SensorData& d){
		float value = std::stof(d.value);
		EXPECT_EQ(d.name, "temperature");
		EXPECT_GT(value, 128);
		EXPECT_LT(value, 129);
	}));

	handlers.back()(mockCallback.getCallback());
}
