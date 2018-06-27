#ifndef THETA_CLOUD_LIGHT_H
#define THETA_CLOUD_LIGHT_H

#include <memory>
#include "SensorData.h"
#include "SensorHandlerToken.h"

class ISL29023;

class ThetaCloudLight
{
public:
	ThetaCloudLight();
	void init();
private:
	std::unique_ptr<ISL29023> isl;
	char dtostrfbuf[15];

	SensorHandlerTokenPtr lightToken;
};

extern ThetaCloudLight thetaCloudLight;

#endif  // THETA_CLOUD_LIGHT_H