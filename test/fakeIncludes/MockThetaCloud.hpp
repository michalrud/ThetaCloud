#pragma once

#include "gmock/gmock.h"
#include <memory>

struct MockThetaCloud
{
    MOCK_METHOD0(init, void());
    MOCK_METHOD0(tick, void());
    MOCK_METHOD1(whenDataAvailable, void(const ThetaCloud::Emit &));
    MOCK_METHOD1(addReadHandler, DeviceHandlerTokenPtr(const ThetaCloud::DeviceReadHandler &));
    MOCK_METHOD2(addWriteHandler,
                 DeviceHandlerTokenPtr(const std::string &topic,
                                       const ThetaCloud::DeviceWriteHandler &));
    MOCK_METHOD1(write, void(const SensorData &));
};

extern std::unique_ptr<MockThetaCloud> mockThetaCloud;
