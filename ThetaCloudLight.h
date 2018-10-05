#ifndef THETA_CLOUD_LIGHT_H
#define THETA_CLOUD_LIGHT_H

#include <memory>
#include "SensorData.h"
#include "DeviceHandlerToken.h"

class ThetaCloud;

class ThetaCloudLight
{
public:
    void init(ThetaCloud& thetaCloud);
private:
    float read();

    DeviceHandlerTokenPtr lightToken;
};

#endif  // THETA_CLOUD_LIGHT_H
