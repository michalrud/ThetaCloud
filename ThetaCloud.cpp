#include "Arduino.h"
#include "ThetaCloud.h"
#include "Wire.h"

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

void ThetaCloud::addReadHandler(const SensorReadHandler& handler)
{
	sensorReadHandlers.push_back(handler);
}

void ThetaCloud::addWriteHandler(const std::string& topic, const SensorWriteHandler& handler)
{
	sensorWriteHandlers[topic] = handler;
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
