#ifndef THETA_CLOUD_I2C_SCANNER_H
#define THETA_CLOUD_I2C_SCANNER_H

#include <string>
#include "DeviceHandlerToken.h"

class ThetaCloudI2CScanner
{
public:
	ThetaCloudI2CScanner();
	void init();
private:
	std::string scan();

	DeviceHandlerTokenPtr token;
};

extern ThetaCloudI2CScanner thetaCloudI2CScanner;

#endif  // THETA_CLOUD_I2C_SCANNER_H
