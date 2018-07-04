#ifndef THETA_CLOUD_DEVICE_HANDLER_TOKEN_H
#define THETA_CLOUD_DEVICE_HANDLER_TOKEN_H

#include <memory>
#include <functional>

class ThetaCloud;

class DeviceHandlerToken
{
    DeviceHandlerToken() = delete;
public:
    ~DeviceHandlerToken()
    {
        deleter();
    }

protected:
    DeviceHandlerToken(const std::function<void()> &deleter) :
        deleter(deleter)
    {
    }

    const std::function<void()> deleter;
    friend ThetaCloud;
};

using DeviceHandlerTokenPtr = std::unique_ptr<DeviceHandlerToken>;

#endif  // THETA_CLOUD_DEVICE_HANDLER_TOKEN_H
