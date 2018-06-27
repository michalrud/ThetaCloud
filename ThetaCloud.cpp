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

SensorHandlerTokenPtr ThetaCloud::addReadHandler(const SensorReadHandler& handler)
{
	auto it = sensorReadHandlers.insert(sensorReadHandlers.end(), handler);
	return std::unique_ptr<SensorHandlerToken>(
		new SensorHandlerToken([it, this](){
			sensorReadHandlers.erase(it);
	}));
}

SensorHandlerTokenPtr ThetaCloud::addWriteHandler(const std::string& topic, const SensorWriteHandler& handler)
{
	auto it = sensorWriteHandlers.insert(std::make_pair(topic, handler));
	assert(it.second);
	auto elementIterator = it.first;
	return std::unique_ptr<SensorHandlerToken>(
		new SensorHandlerToken([elementIterator, this](){
			sensorWriteHandlers.erase(elementIterator);
	}));
}

void ThetaCloud::write(const SensorData& data) const
{
	auto handler = sensorWriteHandlers.find(data.name);
	if (handler != sensorWriteHandlers.end())
	{
		handler->second(data, dataCallback);
	}
}

void ThetaCloud::tick()
{
	if (!initialized) return;
	for (const auto& handler : sensorReadHandlers) {
		handler(this->dataCallback);
	}
}

ThetaCloud thetaCloud;
