#include <ThetaCloud.h>
#include "MockThetaCloud.hpp"
#include <memory>

void ThetaCloud::init()
{
	mockThetaCloud->init();
}

void ThetaCloud::whenDataAvailable(const std::function<void(const SensorData&)>& dataCallback)
{
	mockThetaCloud->whenDataAvailable(dataCallback);
}

DeviceHandlerTokenPtr ThetaCloud::addReadHandler(const DeviceReadHandler& handler)
{
	return mockThetaCloud->addReadHandler(handler);
}

DeviceHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const DeviceWriteHandler& handler)
{
	return mockThetaCloud->addWriteHandler(topic, handler);
}

void ThetaCloud::write(const SensorData& data) const
{
	mockThetaCloud->write(data);
}

void ThetaCloud::tick()
{
	mockThetaCloud->tick();
}

ThetaCloud thetaCloud;
std::unique_ptr<MockThetaCloud> mockThetaCloud;
