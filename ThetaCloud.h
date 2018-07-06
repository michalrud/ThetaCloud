#ifndef THETA_CLOUD_H
#define THETA_CLOUD_H

#include <map>
#include <list>
#include <memory>
#include <functional>
#include "SensorData.h"
#include "DeviceHandlerToken.h"

class ThetaCloud
{
public:
    using Emit = std::function<void(const SensorData &)>;
    using DeviceReadHandler = std::function<void(const Emit &)>;
    using DeviceWriteHandler = std::function<void(const SensorData &, const Emit &)>;
    ThetaCloud() :
        dataCallback([](const SensorData &){
    }),
        initialized(false)
    {
    }

    void init();
    void whenDataAvailable(const Emit &dataCallback_);
    DeviceHandlerTokenPtr addReadHandler(const DeviceReadHandler &handler);
    DeviceHandlerTokenPtr addWriteHandler(const std::string &topic,
                                          const DeviceWriteHandler &handler);
    void write(const SensorData &data) const;
    void emit(const SensorData &data) const;
    void tick();
private:

    Emit dataCallback;
    bool initialized;
    std::list<DeviceReadHandler> deviceReadHandlers;
    std::map<std::string, DeviceWriteHandler> deviceWriteHandlers;
};

extern ThetaCloud thetaCloud;

#endif  // THETA_CLOUD_H
