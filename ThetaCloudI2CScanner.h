#ifndef THETA_CLOUD_I2C_SCANNER_H
#define THETA_CLOUD_I2C_SCANNER_H

#include <string>
#include "DeviceHandlerToken.h"

class ThetaCloud;

class ThetaCloudI2CScanner
{
public:
    void init(ThetaCloud& thetaCloud);
private:
    std::string scan();

    DeviceHandlerTokenPtr token;
};

#endif  // THETA_CLOUD_I2C_SCANNER_H
