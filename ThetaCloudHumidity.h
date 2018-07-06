#ifndef THETA_CLOUD_HUMIDITY_H
#define THETA_CLOUD_HUMIDITY_H

#include "Arduino.h"
#include <memory>
#include "DeviceHandlerToken.h"

class ThetaCloud;

class ThetaCloudHumidity
{
public:
    void init(ThetaCloud& thetaCloud);
protected:
    struct SensorGetValue
    {
        uint16_t value;
        bool error;
    };
    static constexpr SensorGetValue ERROR = {0, true};

    static float GetHumidity();
    static float GetTemperature();

    static SensorGetValue GetValue(uint8_t command);

    DeviceHandlerTokenPtr humidityToken;
    DeviceHandlerTokenPtr temperatureToken;
};

#endif  // THETA_CLOUD_HUMIDITY_H
