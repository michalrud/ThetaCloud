#ifndef THETA_CLOUD_PIR_H
#define THETA_CLOUD_PIR_H

#include <array>
#include <string>
#include "ThetaCloud.h"
#include "DeviceHandlerToken.h"

class ThetaCloudPIR
{
public:
    void init(ThetaCloud& thetaCloud);
    static const std::string MOTION_DETECTED;
    static const std::string MOTION_LOST;
protected:
    struct PirInfo
    {
        PirInfo(const uint8_t pin_, const std::string &name_) :
            pin(pin_),
            name(name_),
            lastState(false)
        {
        }

        bool operator<(const PirInfo &other) const
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
    void handlePir(PirInfo &pir, const ThetaCloud::Emit &emit);
};

#endif  // THETA_CLOUD_PIR_H
