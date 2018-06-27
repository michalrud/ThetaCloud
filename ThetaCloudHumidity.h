#ifndef THETA_CLOUD_HUMIDITY_H
#define THETA_CLOUD_HUMIDITY_H

#include "Arduino.h"
#include <memory>
#include "SensorHandlerToken.h"

class ThetaCloudHumidity
{
public:
	ThetaCloudHumidity();
	void init();
protected:
	struct SensorGetValue
	{
		uint16_t value;
		bool error;
	};

	static float GetHumidity();
	static float GetTemperature();

	static SensorGetValue GetValue(uint8_t command);

	SensorHandlerTokenPtr humidityToken;
	SensorHandlerTokenPtr temperatureToken;
};

extern ThetaCloudHumidity thetaCloudHumidity;

#endif  // THETA_CLOUD_HUMIDITY_H