#ifndef THETA_CLOUD_PIR_H
#define THETA_CLOUD_PIR_H

#include <array>
#include <string>
#include "ThetaCloud.h"
#include "DeviceHandlerToken.h"

class ThetaCloudPIR
{
public:
	ThetaCloudPIR();
	void init();
	static const std::string MOTION_DETECTED;
	static const std::string MOTION_LOST;
protected:
	struct PirInfo
	{
		PirInfo(const uint8_t pin, const std::string& name) :
			pin(pin), name(name), lastState(false)
		{}
		bool operator<(const PirInfo& other) const
		{
			return this->pin < other.pin;
		}
		const uint8_t pin;
		const std::string name;
		bool lastState;
		DeviceHandlerTokenPtr token;
	};
	std::array<PirInfo, 2> PIR_GPIO { {  // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65815
		PirInfo(12, "PIR1"),
		PirInfo(13, "PIR2")
	} };
	void handlePir(PirInfo& pir, const ThetaCloud::Emit& emit);
	bool enabled;
};

extern ThetaCloudPIR thetaCloudPIR;

#endif  // THETA_CLOUD_PIR_H
