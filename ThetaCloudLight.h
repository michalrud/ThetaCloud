#ifndef THETA_CLOUD_LIGHT_H
#define THETA_CLOUD_LIGHT_H

#include <memory>
#include "SensorData.h"
#include "DeviceHandlerToken.h"

class ThetaCloudLight
{
public:
    ThetaCloudLight();
    void init();
private:
    float read();

    DeviceHandlerTokenPtr lightToken;
};

extern ThetaCloudLight thetaCloudLight;

#endif  // THETA_CLOUD_LIGHT_H
