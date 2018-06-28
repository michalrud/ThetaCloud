#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "I2CHelpers.h"
#include "Wire.h"
#include "testing_utils.hpp"

using ::testing::Return;

struct I2CHelpersTests : public GenericTest
{
	::testing::Sequence s;
};

const uint8_t TEST_ADDRESS = 0x12;
const uint8_t TEST_DATA = 0x34;

const uint8_t TEST_ADDRESS2 = 0x56;
const uint8_t TEST_DATA2 = 0x78;

const uint8_t TEST_ADDRESS3 = 0x90;
const uint8_t TEST_DATA3 = 0x13;

TEST_F(I2CHelpersTests, WritingToI2C)
{
	EXPECT_CALL((*Wire.mock), beginTransmission(TEST_ADDRESS)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(TEST_DATA)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	WriteToI2C(TEST_ADDRESS).write(TEST_DATA);

	EXPECT_CALL((*Wire.mock), beginTransmission(TEST_ADDRESS2)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(TEST_DATA2)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	WriteToI2C(TEST_ADDRESS2).write(TEST_DATA2);

	EXPECT_CALL((*Wire.mock), beginTransmission(TEST_ADDRESS3)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(TEST_DATA)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(TEST_DATA2)).InSequence(s);
	EXPECT_CALL((*Wire.mock), write(TEST_DATA3)).InSequence(s);
	EXPECT_CALL((*Wire.mock), endTransmission()).InSequence(s);
	WriteToI2C(TEST_ADDRESS3).write(TEST_DATA).write(TEST_DATA2).write(TEST_DATA3);
}

TEST_F(I2CHelpersTests, ReadingFromI2C_success)
{
	EXPECT_CALL((*Wire.mock), requestFrom(TEST_ADDRESS, 5)).InSequence(s);
	EXPECT_CALL((*Wire.mock), available()).InSequence(s).WillOnce(Return(5));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0A));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0B));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0C));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0D));
	EXPECT_CALL((*Wire.mock), read()).InSequence(s).WillOnce(Return(0x0E));
	auto readResult = ReadFromI2C<5>(TEST_ADDRESS);
	EXPECT_FALSE(readResult.second);
	EXPECT_THAT(readResult.first, ::testing::ElementsAre(0x0A, 0x0B, 0x0C, 0x0D, 0x0E));
}

TEST_F(I2CHelpersTests, ReadingFromI2C_failure)
{
	EXPECT_CALL((*Wire.mock), requestFrom(TEST_ADDRESS, 5));
	EXPECT_CALL((*Wire.mock), available()).WillRepeatedly(Return(0));
	EXPECT_CALL((*Wire.mock), read()).Times(0);
	EXPECT_CALL((*arduinoMock), delay(::testing::_)).Times(10);
	auto readResult = ReadFromI2C<5>(TEST_ADDRESS);
	EXPECT_TRUE(readResult.second);
}
