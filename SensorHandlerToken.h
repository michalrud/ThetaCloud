#ifndef THETA_CLOUD_SENSOR_HANDLER_TOKEN_H
#define THETA_CLOUD_SENSOR_HANDLER_TOKEN_H

#include <memory>
#include <functional>

class ThetaCloud;

class SensorHandlerToken {
	SensorHandlerToken() = delete;
public:
	~SensorHandlerToken() { deleter(); }
protected:
	SensorHandlerToken(const std::function<void()>& deleter) :
		deleter(deleter)
	{}
	const std::function<void()> deleter;
	friend ThetaCloud;
};

using SensorHandlerTokenPtr = std::unique_ptr<SensorHandlerToken>;

#endif  // THETA_CLOUD_SENSOR_HANDLER_TOKEN_H