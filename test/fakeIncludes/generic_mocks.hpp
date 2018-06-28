#pragma once

#include "gmock/gmock.h"

struct MockCallback
{
	MOCK_METHOD1(callback, void(const SensorData& data));
};

struct MockReadHandler
{
	MOCK_METHOD1(readHandler, void(const ThetaCloud::Emit& emit));
};

struct MockWriteHandler
{
	MOCK_METHOD2(writeHandler, void(const SensorData& data, const ThetaCloud::Emit& emit));
};
