#include "Arduino.h"
#include "ThetaCloud.h"
#include "Wire.h"
#include <cassert>

const int TC_I2C_SDA = 2;
const int TC_I2C_SCL = 14;

void ThetaCloud::init()
{
	// Initialize I2C
	Wire.begin(TC_I2C_SDA, TC_I2C_SCL);
	initialized = true;
}

void ThetaCloud::whenDataAvailable(const std::function<void(const SensorData&)>& dataCallback)
{
	this->dataCallback = dataCallback;
}

DeviceHandlerTokenPtr ThetaCloud::addReadHandler(const DeviceReadHandler& handler)
{
	auto it = deviceReadHandlers.insert(deviceReadHandlers.end(), handler);
	return std::unique_ptr<DeviceHandlerToken>(
		new DeviceHandlerToken([it, this](){
			deviceReadHandlers.erase(it);
	}));
}

DeviceHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const DeviceWriteHandler& handler)
{
	auto it = deviceWriteHandlers.insert(std::make_pair(topic, handler));
	assert(it.second);
	auto elementIterator = it.first;
	return std::unique_ptr<DeviceHandlerToken>(
		new DeviceHandlerToken([elementIterator, this](){
			deviceWriteHandlers.erase(elementIterator);
	}));
}

void ThetaCloud::write(const SensorData& data) const
{
	auto handler = deviceWriteHandlers.find(data.name);
	if (handler != deviceWriteHandlers.end())
	{
		handler->second(data, dataCallback);
	}
}

void ThetaCloud::tick()
{
	if (!initialized) return;
	for (const auto& handler : deviceReadHandlers) {
		handler(this->dataCallback);
	}
}

ThetaCloud thetaCloud;
