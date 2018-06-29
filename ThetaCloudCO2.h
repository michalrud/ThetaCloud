#ifndef THETA_CLOUD_CO2_H
#define THETA_CLOUD_CO2_H

#include <string>
#include "DeviceHandlerToken.h"

class ThetaCloudCO2
{
public:
	ThetaCloudCO2();
	void init();
protected:
	struct SensorGetValue
	{
		float co2Value;
		float vocValue;
		bool error;
	};
	static constexpr ThetaCloudCO2::SensorGetValue ERROR = {0, 0, true};
	bool enabled;
	static SensorGetValue GetCo2Level();

	DeviceHandlerTokenPtr co2Token;
};

extern ThetaCloudCO2 thetaCloudCO2;

#endif  // THETA_CLOUD_CO2_H
