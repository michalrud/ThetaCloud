#pragma once

#include "gmock/gmock.h"
#include <memory>

struct MockThetaCloud
{
	MOCK_METHOD0(init, void());
	MOCK_METHOD0(tick, void());
	MOCK_METHOD1(whenDataAvailable, void(const ThetaCloud::Emit&));
	MOCK_METHOD1(addReadHandler, SensorHandlerTokenPtr(const ThetaCloud::SensorReadHandler&));
	MOCK_METHOD2(addWriteHandler, SensorHandlerTokenPtr(const std::string& topic, const ThetaCloud::SensorWriteHandler&));
	MOCK_METHOD1(write, void(const SensorData&));
};

extern std::unique_ptr<MockThetaCloud> mockThetaCloud;