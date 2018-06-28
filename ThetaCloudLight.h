#ifndef THETA_CLOUD_LIGHT_H
#define THETA_CLOUD_LIGHT_H

#include <memory>
#include "SensorData.h"
#include "SensorHandlerToken.h"

class ThetaCloudLight
{
public:
	ThetaCloudLight();
	void init();
private:
	float read();
	char dtostrfbuf[15];

	SensorHandlerTokenPtr lightToken;
};

extern ThetaCloudLight thetaCloudLight;

#endif  // THETA_CLOUD_LIGHT_H