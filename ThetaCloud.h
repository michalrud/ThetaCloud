#ifndef THETA_CLOUD_H
#define THETA_CLOUD_H

#include <map>
#include <list>
#include <memory>
#include <functional>
#include "SensorData.h"

#define THETA_SUBSCRIBE() \
	void thetaSubscribeFunction(const SensorData& sensorData);\
	ThetaSubscriber ts(thetaCloud, &thetaSubscribeFunction);\
	void thetaSubscribeFunction(const SensorData& sensorData)

class ThetaCloud
{
public:
	using Emit = std::function<void(const SensorData&)>;
	using SensorReadHandler = std::function<void(const Emit&)>;
	using SensorWriteHandler = std::function<void(const SensorData&, const Emit&)>;
	ThetaCloud() :
		dataCallback([](const SensorData&){}),
		initialized(false)
	{}
	~ThetaCloud() {}

	void init();
	void whenDataAvailable(const Emit& dataCallback);
	void addReadHandler(const SensorReadHandler& handler);
	void addWriteHandler(const std::string& topic, const SensorWriteHandler& handler);
	void write(const SensorData& data) const;
	void tick();
private:
	Emit dataCallback;
	bool initialized;
	std::list<SensorReadHandler> sensorReadHandlers;
	std::map<std::string, SensorWriteHandler> sensorWriteHandlers;
};

extern ThetaCloud thetaCloud;

#endif  // THETA_CLOUD_H