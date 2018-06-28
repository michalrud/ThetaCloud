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

SensorHandlerTokenPtr ThetaCloud::addReadHandler(const SensorReadHandler& handler)
{
	return mockThetaCloud->addReadHandler(handler);
}

SensorHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const SensorWriteHandler& handler)
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
