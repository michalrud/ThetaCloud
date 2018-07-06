#ifndef THETA_CLOUD_RELAY_H
#define THETA_CLOUD_RELAY_H

#include "SensorData.h"
#include "DeviceHandlerToken.h"
#include <map>

class ThetaCloudRelay
{
public:
    void init();

    static const char *RELAY1;
    static const char *RELAY2;
    static const std::string ON;
    static const std::string OFF;
protected:
    enum class Switch {
        one, two
    };
    const std::map<Switch, int> SWITCH_GPIO = {
        {Switch::one, 12},
        {Switch::two, 13}
    };
    void trySetSwitch(Switch sw, const SensorData &data) const;
    void setSwitch(Switch whichOne, bool enabled) const;

    DeviceHandlerTokenPtr switch1Token;
    DeviceHandlerTokenPtr switch2Token;
};

extern ThetaCloudRelay thetaCloudRelay;

#endif  // THETA_CLOUD_RELAY_H
