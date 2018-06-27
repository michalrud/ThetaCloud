#ifndef THETA_CLOUD_CO2_H
#define THETA_CLOUD_CO2_H

#include <string>

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
	bool enabled;
	static SensorGetValue GetCo2Level();
};

extern ThetaCloudCO2 thetaCloudCO2;

#endif  // THETA_CLOUD_CO2_H