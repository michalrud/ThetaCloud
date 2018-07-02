#ifndef THETA_CLOUD_ENVIRONMENT_H
#define THETA_CLOUD_ENVIRONMENT_H

#include <string>
#include "DeviceHandlerToken.h"

class ThetaCloudEnvironment
{
public:
	ThetaCloudEnvironment();
	void init();
protected:
	struct AirSensorValue
	{
		float co2Value;
		float vocValue;
		bool error;
	};
	static constexpr ThetaCloudCO2::SensorGetValue ERROR = {0, 0, true};
	static constexpr ThetaCloudEnvironment::AirSensorValue AIR_ERROR = {0, 0, true};
	bool enabled;
	static AirSensorValue GetAirReadings();

	DeviceHandlerTokenPtr co2Token;
};

extern ThetaCloudEnvironment thetaCloudEnvironment;

#endif  // THETA_CLOUD_ENVIRONMENT_H
