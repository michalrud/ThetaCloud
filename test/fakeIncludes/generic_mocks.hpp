#pragma once

#include "gmock/gmock.h"
#include "ThetaCloud.h"

struct NiceMockCallback
{
	MOCK_METHOD1(callback, void(const SensorData& data));
	std::function<void(const SensorData&)> getCallback()
	{
		return [this](const SensorData& d)
		{
			callback(d);
		};
	};
};

using MockCallback = ::testing::StrictMock<NiceMockCallback>;

struct NiceMockReadHandler
{
	MOCK_METHOD1(readHandler, void(const ThetaCloud::Emit& emit));
};
using MockReadHandler = ::testing::StrictMock<NiceMockReadHandler>;

struct NiceMockWriteHandler
{
	MOCK_METHOD2(writeHandler, void(const SensorData& data, const ThetaCloud::Emit& emit));
};
using MockWriteHandler = ::testing::StrictMock<NiceMockWriteHandler>;
