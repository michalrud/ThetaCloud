#ifndef THETA_CLOUD_ENVIRONMENT_H
#define THETA_CLOUD_ENVIRONMENT_H

/* Uncomment to use MPL3115A2 sensor.
 * Adafruit's MPL3115A2 Arduino library needs to be installed.
 */
// #define USE_MPL3115A2

#include <string>
#include "DeviceHandlerToken.h"

#ifdef USE_MPL3115A2
#ifndef MPL3115A2_ADDRESS // this library is missing an include guard...
#include <Adafruit_MPL3115A2.h>
#endif // MPL3115A2_ADDRESS
#endif // USE_MPL3115A2

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
	static constexpr ThetaCloudEnvironment::AirSensorValue AIR_ERROR = {0, 0, true};
	bool enabled;
	static AirSensorValue GetAirReadings();
#ifdef USE_MPL3115A2
	struct BaroSensorValue
	{
		float pressure;
		float altitude;
		float temperature;
		bool error;
	};
	static constexpr ThetaCloudEnvironment::BaroSensorValue BARO_ERROR = {0, 0, 0, true};
	BaroSensorValue GetBaroReadings();
	Adafruit_MPL3115A2 baroSensor;
#endif

	DeviceHandlerTokenPtr co2Token;
};

extern ThetaCloudEnvironment thetaCloudEnvironment;

#endif  // THETA_CLOUD_ENVIRONMENT_H
