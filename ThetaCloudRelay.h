#ifndef THETA_CLOUD_RELAY_H
#define THETA_CLOUD_RELAY_H

#include "SensorData.h"
#include "SensorHandlerToken.h"
#include <map>

class ThetaCloudRelay
{
public:
	ThetaCloudRelay();
	void init();
protected:
	bool enabled;
	enum class Switch {one, two};
	const std::map<Switch, int> SWITCH_GPIO = {
		{Switch::one, 12},
		{Switch::two, 13}
	};
	void trySetSwitch(Switch sw, const SensorData& data) const;
	void setSwitch(Switch whichOne, bool enabled) const;

	SensorHandlerTokenPtr switch1Token;
	SensorHandlerTokenPtr switch2Token;
};

extern ThetaCloudRelay thetaCloudRelay;

#endif  // THETA_CLOUD_RELAY_H